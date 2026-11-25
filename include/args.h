#pragma once

#include <stdint.h>

// size will be 8ch* + 2int + 1cha + 1int (12, until 2 * 8 = 16 we need 6 pad) + 6pad = 16
struct args {
  char *command;
  uint16_t amount; // max is 2**sizeof(uint16_t) - 1 = 2**16 - 1 = 65535 */
  char type;
  uint8_t is_lonely;
};

void parse_args(int argc, char **argv, struct args *args);
