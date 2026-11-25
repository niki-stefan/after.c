#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_args(int argc, char **argv, struct args *args) {
  uint8_t i, cmdlen;
  char type;

  args->data = 0;
  args->amount = 0;

  // The first argument is the file name, so skip it
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-cmd") == 0) {
      // Last element
      if (argc - i == 1) {
        puts("Specify a command after.c -cmd");

        exit(1);
      }

      cmdlen = strlen(argv[i + 1]);

      if (cmdlen >= 63) {
        puts("Command cannot be longer than 63 letters");

        exit(1);
      }

      strcpy(args->command, argv[++i]);
      args->data = cmdlen << 2;

      continue;
    } else if (strcmp(argv[i], "-lonely") == 0) {
      args->data |= 1; // Set the last bit to 1
    } else if (argv[i][0] > '0' && argv[i][0] < '9') {
      sscanf(argv[i], "%hhu%c", &args->amount, &type);

      args->data |= (type == 'm') << 1;

      if (type != 's' && type != 'm') {
        printf("What is \"%hhu%c\" bro?\n", args->amount, type);

        exit(1);
      }
    } else {
      printf("What is \"%s\" bro?\n", argv[i]);

      exit(1);
    }
  }
}
