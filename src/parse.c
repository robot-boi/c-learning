#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

// void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees)
// {}

// int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees,
//                  char *addstring) {}

// int read_employees(int fd, struct dbheader_t *dbhdr,
//                    struct employee_t **employeesOut) {}

int output_file(int fd, struct dbheader_t *dbhdr,
                struct employee_t *employees) {

  if (fd < 0) {
    perror("Invalid file descriptor");
    return STATUS_ERROR;
  }

  dbhdr->magic = htonl(dbhdr->magic);
  dbhdr->filesize = htonl(dbhdr->filesize);
  dbhdr->count = htons(dbhdr->count);
  dbhdr->version = htons(dbhdr->version);

  lseek(fd, 0, SEEK_SET);
  write(fd, dbhdr, sizeof(struct dbheader_t));

  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
  if (fd < 0) {
    perror("Invalid file descriptor");
    return STATUS_ERROR;
  }

  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Error allocating memory for db header\n");
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct dbheader_t)) !=
      sizeof(struct dbheader_t)) {
    perror("read");
    free(header);
    return STATUS_ERROR;
  }

  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  if (header->magic != HEADER_MAGIC) {
    printf("Invalid magic number: expected %x, got %x\n", HEADER_MAGIC,
           header->magic);
    free(header);
    return STATUS_ERROR;
  }

  if (header->version != 1) {
    printf("Unsupported version: expected 0x1, got 0x%x\n", header->version);
    free(header);
    return STATUS_ERROR;
  }

  struct stat dbstat = {0};
  fstat(fd, &dbstat);
  if (header->filesize != dbstat.st_size) {
    printf("File size mismatch: expected %u, got %ld\n", header->filesize,
           dbstat.st_size);
    free(header);
    return STATUS_ERROR;
  }
  *headerOut = header;
  return STATUS_SUCCESS;
}

int create_db_header(struct dbheader_t **headerOut) {
  if (headerOut == NULL) {
    printf("Invalid header output pointer\n");
    return STATUS_ERROR;
  }

  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));

  if (header == NULL) {
    printf("Error allocating memory for db header\n");
    return STATUS_ERROR;
  }

  header->version = 0x1;
  header->count = 0;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);

  *headerOut = header;
  return STATUS_SUCCESS;
}
