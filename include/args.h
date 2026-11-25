#pragma once

#include <stdint.h>

struct args {
  uint8_t amount; // max is 2**sizeof(uint8_t) - 1 = 2**8 - 1 = 255
  uint8_t data; // ABCDEFXY, ABCDEF - command length (max 2^6 - 1 = 63), X - 0 => s; 1 => m, Y = is_lonely
  char command[63];
};

void parse_args(int argc, char **argv, struct args *args);
