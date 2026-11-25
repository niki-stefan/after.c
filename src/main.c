#include "args.h"
#include "magic_numbers.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios old_term, new_term;
static uint16_t remaining; // in seconds
static uint8_t status;     // 000000AB, A - is_paused, B - has_executed_command

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

  if ((args.data >> 1) & 1) {
    remaining = start_time = args.amount * 60;
  } else {
    remaining = start_time = args.amount;
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

  while (~args.data & 1) {
    read(STDIN_FILENO, &ch, 1);

    if (remaining == 0 && !(status & 1)) {
      if ((args.data >> 2) > 0) {
        system(args.command);
      } else {
        system(DEFAULT_COMMAND);
      }

      status = 3; // 00000011 = is paused and has executed
    }

    if (ch == 'q') {
      break;
    } else if (ch == 'r') {
      remaining = start_time;
      status = 0;
      alarm(1);
    } else if (ch == 'j' && remaining > 5) {
      remaining -= 5;

      // We also check that start_time > 5, because it is unsigned, otherwise
      // it would overflow and becone n^16 - 1, which then would ofcourse be
      // bigger than remaining
    } else if (ch == 'k' && start_time > 5 && remaining < start_time - 5) {
      remaining += 5;

      status = 0; // 00000000, meaning, resume the timer and reset has_executed
      alarm(1);
    } else if (ch == 'J' && remaining > 60) {
      remaining -= 60;
    } else if (ch == 'K' && start_time > 60 && remaining < start_time - 60) {
      remaining += 60;

      status = 0; // 00000000, meaning, resume the timer and reset has_executed
      alarm(1);
    } else if (ch == ' ') {
      /* If its already paused, it means we want it resumed, status will be
         0000001B, so alarm(1) will start a timer for one second
         If its not already paused, it means we want it paused, status will be
         0000000B, and alarm(0) will cancel any pending alarm
         https://man.archlinux.org/man/alarm.2.en
         Only problem is that if you paused with, for example, 200ms before the
         secound would pass, when resuming it would wait a full second, thus
         adding 800ms in plus, but its minuscule */
      alarm(status >> 1);

      status ^= 1 << 1; // 00000010, will flip the A bit (the is_paused)
    }

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

  if (remaining != 0 && ~status >> 1)
    alarm(1);
}
