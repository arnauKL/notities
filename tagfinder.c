// tagfinder: find tags (i.e.: #todo)
// recreate obsidian's functionality
//
// prints all of the files with the desired tag

// could implement some caching

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define USAGE          "Usage: tagfinder <tag name>"
#define PROGRAM_NAME   "tagfinder"
#define FILE_EXTENSION "[*.md]"
#define BUFFER_SIZE    4096
// #define TAG_DELIMITER  " \t\r\n\a,.;:-*_!?()[]{}"

/**
 * For now can only find '#tag' type, but
 * will add yaml header tags in the future
 */

enum ParserState
{
    STATE_TEXT,
    STATE_TAG
};

void parse_file(int file_descriptor);

int
main(int argc, char **argv)
{
    int file_descriptor;
    // char *dir_path;
    // if (argc == 3) {
    //     dir_path = argv[2];
    // } else {
    //     dir_path = ".";
    if (argc != 2) {
        fprintf(stderr, "%s\n", USAGE);
        return EXIT_FAILURE;
    }
    // }

    // look in all the files in the directory:
    glob_t results;
    int ret = glob("*.md", 0, NULL, &results);

    if (ret == 0) {

        for (size_t i = 0; i < results.gl_pathc; i++) {

            file_descriptor = open(results.gl_pathv[i], O_RDONLY);

            printf("parsing %s...\n", results.gl_pathv[i]);

            if (file_descriptor < 0) {
                fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, argv[1], strerror(errno));
                continue;
            }

            parse_file(file_descriptor);
            close(file_descriptor);
        }

        globfree(&results);
    } else {
        // Could not open directory
        perror("");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void
parse_file(int fildes)
{
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read = 0;
    errno = 0;
    enum ParserState state = STATE_TEXT;

    while ((bytes_read = read(fildes, buffer, sizeof(buffer))) > 0) {

        // Scan blocks
        for (int i = 0; i < bytes_read; i++) {

            // State machine
            switch (state) {

            case STATE_TEXT:
                if (buffer[i] == '#') {
                    state = STATE_TAG;
                }
                break;
            case STATE_TAG:
                if (isspace(buffer[i]) || buffer[i] == '.') {
                    state = STATE_TEXT;
                    printf("\n");
                } else {
                    printf("%c", buffer[i]);
                }
                break;
            }
        }
    }
    printf("\n");
}
