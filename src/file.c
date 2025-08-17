#include "file.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

int create_db_file(char *filename) {
    int fd = open(filename, O_RDWR);
    if (fd != -1) {
        // File already exists, close the file descriptor and return an error
        close(fd);
        printf("File already exists.\n");
        return STATUS_ERROR;
    }

    fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    }

    return fd; // Return success
}

int open_db_file(char *filename) {
    int fd = open(filename, O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    }
    return fd; // Return the file descriptor
}
