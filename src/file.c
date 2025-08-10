#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "file.h"

int create_db_file(char *filename) {
  int fd = open(filename, O_RDWR);
  if (fd != -1) {
    // File already exists, close the file descriptor and return an error
    close(fd);
    printf("File %s already exists.\n", filename);
    return STATUS_ERROR;
  }

  fd = open(filename, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }

  // Optionally, you can write initial data to the file here
  // For example, writing a header or metadata

  close(fd);
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
