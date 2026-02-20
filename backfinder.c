// Backfinder: find backlinks ([[a_file_name|a possible title]])
// recreate obsidian's functionality
//
// prints all the backlinks found in that file

#include <errno.h>
#include <fcntl.h> //open
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> //open
#include <unistd.h>

#define USAGE          "Usage: backfinder <filename>"
#define PROGRAM_NAME   "backfinder"
#define FILE_EXTENSION ".md"
#define BUFFER_SIZE    4096

int
main(int argc, char **argv)
{
    int file_descriptor;
    if (argc > 2) {
        fprintf(stderr, "%s\n", USAGE);
        return EXIT_FAILURE;
    } else if (argc == 1) {
        file_descriptor = STDIN_FILENO;
    } else {
        // try to open the file
        file_descriptor = open(argv[1], O_RDONLY);
    }

    if (file_descriptor < 0) {
        fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, argv[1], strerror(errno));
        return EXIT_FAILURE;
    }

    // file descriptor opened, now read ze file

    unsigned char buffer[BUFFER_SIZE];

    int bytes_read = 0;
    errno = 0;

    int depth = 0; // number of closing brackets found

    while ((bytes_read = read(file_descriptor, buffer, sizeof(buffer))) > 0) {

        for (int i = 0; i < bytes_read; i++) {
            if (depth == 2) {
                if (buffer[i] != ']') {
                    printf("%c", buffer[i]);
                } else {
                    printf("\n");
                    depth--;
                }
            } else if (buffer[i] == '[') {
                depth++;
            } else if (buffer[i] == ']') {
                depth--;
            }
        }
    }
    printf("\n");

    if (depth != 0) {
        printf("Wrong syntax\n");
    }

    return EXIT_SUCCESS;
}
