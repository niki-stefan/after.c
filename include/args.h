#pragma once

#include <stdint.h>

struct args {
  uint16_t amount; // max is 2**sizeof(uint16_t) - 1 = 2**16 - 1 = 65535
  char type; // s - seconds, m - minutes
  char command[50];
  uint8_t is_lonely; // non-interactive
};

void parse_args(int argc, char **argv, struct args *args);
