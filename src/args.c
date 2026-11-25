#include "args.h"
#include "magic_numbers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_args(int argc, char **argv, struct args *args) {
  uint8_t i, cmdlen;

  args->is_lonely = 0;

  // The first argument is the file name, so skip it
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-cmd") == 0) {
      // Last element
      if (argc - i == 1) {
        puts("Specify a command after.c -cmd");

        exit(1);
      }

      cmdlen = strlen(argv[i + 1]);

      args->command = malloc(cmdlen + 1);
      strcpy(args->command, argv[++i]);
      args->command[cmdlen - 1] = '\0';

      continue;
    } else if (strcmp(argv[i], "-lonely") == 0) {
      args->is_lonely = 1;
    } else if (argv[i][0] > '0' && argv[i][0] < '9') {
      sscanf(argv[i], "%hu%c", &args->amount, &args->type);

      if (args->type != 's' && args->type != 'm') {
        printf("What is \"%hhu%c\" bro?\n", args->amount, args->type);

        exit(1);
      }
    } else {
      printf("What is \"%s\" bro?\n", argv[i]);

      exit(1);
    }
  }

  if (args->amount == 0) {
    puts("That ended yesterday! (amount to wait was 0)");

    exit(1);
  }

  if (args->command == NULL) {
    cmdlen = strlen(DEFAULT_COMMAND);

    args->command = malloc(cmdlen + 1);
    strcpy(args->command, DEFAULT_COMMAND);
    args->command[cmdlen] = '\0';
  }
}
