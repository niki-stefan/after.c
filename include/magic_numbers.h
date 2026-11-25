#define RED "\x1B[38;5;167m"
#define BLUE "\x1B[38;5;29m"
#define YELLOW "\x1B[38;5;143m"
#define PURPLE "\x1B[38;5;96m"
#define RESET "\x1B[0m"

#define SHOW_CURSOR "\x1B[?25h"
#define HIDE_CURSOR "\x1B[?25l"

#define DEFAULT_COMMAND "/home/niki/dev/scripts/alarm.sh"

#define CONTROLS                                                               \
  RED "j" BLUE " -5s " RED "k" BLUE " +5s " RED "J" BLUE " -1m " RED "K" BLUE  \
      " +1m " RED "space" BLUE " pause " RED "r" BLUE " reset " RED "q" BLUE " quit :(" RESET

