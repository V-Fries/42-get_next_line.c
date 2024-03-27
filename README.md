# 42-get-next-line.c

## Description:
get_next_line is a project in the 42 curriculum aimed at improving students' understanding of file I/O operations.  
The goal is to create a function that enables its user to read a file line by line.

## Usage:
```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "get_next_line.h"


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Please provide at least 1 argument\n");
        return 1;
    }

    const int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open file \"%s\"\n", argv[1]);
        return 2;
    }

    char* line = get_next_line(fd);
    while (line != NULL) {
        // Process the line
        printf("%s", line);

        free(line);
        line = get_next_line(fd);
    }
    // Should check errno value here as get_next_line() returns NULL both
    // when the file was fully read and when an error occurred

    close(fd);
}

```
Note: Remember to handle memory allocation properly, freeing all `char*` returned by get_next_line() when it's no longer needed.
