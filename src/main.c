#include "args.h"
#include "magic_numbers.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios old_term, new_term;
static uint16_t remaining; // in seconds

static void bye(void);
static void handle_interupt(int signal_num);
static void init_term();
static void handle_alarm_signal(int signal);

int main(int argc, char **argv) {
  struct args args;
  char ch;

  uint16_t start_time = 0;

  init_term();

  parse_args(argc, argv, &args);

  if (args.type == 's') {
    remaining = start_time = args.amount;
  } else {
    remaining = start_time = args.amount * 60;
  }

  atexit(bye);

  printf(HIDE_CURSOR);
  printf(YELLOW "after.c" PURPLE " %02hu:%02hu    " CONTROLS "\n" RED
                "  00:00 remaning\n" BLUE "  00:00 passed" RESET,
         start_time / 60, start_time % 60);

  // printf flushes at \n
  fflush(stdout);

  // Notice: its alrm not alArm 😃
  // https://stackoverflow.com/questions/3879230/blocking-non-blocking-timer-in-c
  signal(SIGALRM, handle_alarm_signal);
  alarm(1);

  // We move the cursor after the time (12 is the start of the line - 5 the
  // clock width)
  write(STDOUT_FILENO, "\x1B[7D", 4);

  while (!args.is_lonely) {
    read(STDIN_FILENO, &ch, 1);

    if (ch == 'q')
      break;

    printf(RED "\x1B[1A\x1B[5D%02hu:%02hu", remaining / 60, remaining % 60);
    printf(BLUE "\x1B[1B\x1B[5D%02hu:%02hu" RESET,
           (start_time - remaining) / 60, (start_time - remaining) % 60);
    fflush(stdout);

    ch = '\0';
  }

  return 0;
}

static void bye(void) {
  write(STDOUT_FILENO, SHOW_CURSOR "\n", 7);

  // Bassically we used termios to: store the old terminal config, change the
  // current one, then restore the old one at exit
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}

static void handle_interupt(int signal_num) {
  bye();

  exit(0);
}

static void init_term() {
  tcgetattr(STDIN_FILENO, &old_term);
  new_term = old_term;

  // https://en.wikibooks.org/wiki/Serial_Programming/termios
  new_term.c_lflag &= ~(ICANON | ECHO | ISIG | ECHONL);
  new_term.c_cc[VMIN] = 0;
  new_term.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

static void handle_alarm_signal(int signal) {
  remaining--;

  if (remaining != 0)
    alarm(1);
}
