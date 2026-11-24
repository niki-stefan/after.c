#include "args.h"

#include <stdio.h>

int main(int argc, char **argv) {
  struct args args;

  parse_args(argc, argv, &args);

  printf("executing %s after.c %hd%c\n", args.command, args.amount, args.type);
  return 0;
}
