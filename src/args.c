#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_args(int argc, char **argv, struct args *args) {
  int i;

  // Defaults
  args->type = 's';
  args->amount = 2;
  strcpy(args->command, "rm /"); // rm = Receive Money 🤗
  args->is_lonely = 0;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-cmd") == 0) {
      // Last element
      if (argc - i == 1) {
        puts("Specify a command after.c -cmd");

        exit(1);
      }

      strcpy(args->command, argv[++i]);
    } else if (strcmp(argv[i], "-lonely") == 0) {
      args->is_lonely = 1;
    } else if (argv[i][0] > '0' && argv[i][0] < '9') {
      sscanf(argv[i], "%hd%c", &args->amount, &args->type);

      if (args->type != 's' && args->type != 'm') {
        printf("What is \"%hd%c\" bro?\n", args->amount, args->type);

        exit(1);
      }
    } else {
      printf("What is \"%s\" bro?\n", argv[i]);

      exit(1);
    }
  }
}
