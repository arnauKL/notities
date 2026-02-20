/*
 * Functions (?):
 *    - call backfinder
 *    - call nvim on a file
 *    - show menu
 *    - parse yaml files
 **/

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACKLINK_CMD "./backlinks"

void
print_options()
{
    printf("1. Show backlinks");
    printf("2. Show backlinks");
}

int
main(int argc, const char **argv)
{
    assert(argc == 2);
    const char *filename = argv[1];

    printf("Options for %s\n", filename);
    print_options();

    int option;

    do {
        scanf("%d", &option);

        switch (option) {
        case 1:
            execv(BACKLINK_CMD, filename);
            break;
        default:
            printf("Wrong option selected\n");
            return EXIT_FAILURE;
        }
    } while (1);

    return EXIT_SUCCESS;
}
