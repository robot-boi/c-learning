#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s [-n] -f <filepath>\n", argv[0]);
  printf("Options:\n");
  printf("  -n            Create a new file\n");
  printf("  -f <filepath> Specify the file path (required)\n");
  return;
}

int main(int argc, char *argv[]) {
  char *filepath = NULL;
  bool newfile = false;
  int c;
  int dbfd = -1;
  struct dbheader_t *dbhdr = NULL;

  while ((c = getopt(argc, argv, "nf:")) != -1) {
    switch (c) {
    case 'n':
      // Handle the -n option
      newfile = true;
      break;
    case 'f':
      // Handle the -f option with optarg
      filepath = optarg;
      break;
    case '?':
      // Handle unknown options
      printf("Unknown option: %c\n", c);
      break;
    default:
      return -1;
    }
  }

  if (filepath == NULL) {
    printf("Filepath is required.\n");
    print_usage(argv);
    return 0;
  }

  if (newfile) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Error creating new db file\n");
      return -1;
    }

    if (create_db_header(&dbhdr) == STATUS_ERROR) {
      printf("Error creating db header\n");
      return -1;
    }
  } else {
    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Error opening db file\n");
      return -1;
    }

    if (validate_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
      printf("Error validating db header\n");
      return -1;
    }
  }

  printf("New file: %d\n", newfile);
  printf("Filepath: %s\n", filepath);

  output_file(dbfd, dbhdr, NULL);
  return 0;
}
