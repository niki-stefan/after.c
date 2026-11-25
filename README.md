# after.c

You ever so invested in C you don't even notice the time pass, and need to be reminded to take a break? Just type `after 30m`, and after 30 minutes an "alarm" will fire.

<img width="1102" height="174" alt="screen_2026-11-25_17-07-09" src="https://github.com/user-attachments/assets/dd77f603-b5e2-4d40-b407-a2c8fddbe070" />

## Syntax

`after %hd[m|s] -cmd "COMMAND" -lonely`

`%hd` is an `uint16_t` (0 <= n <= 65535), `-lonely` means it will not be interactive, it will get out of your way.

For example, after 40 seconds send a notification: `after 40s -cmd "notify-send hi" -lonely`, set a timer to study for 30 minutes: `after 30m` (defaults to *not* lonely, and the defalt command can be changed in include/magic_numbers.h, note that the default command is set for my use case, and *most probably* wont work for you)

## What it uses

When it is not lonely, it uses the **non-blocking** [alarm](https://man.archlinux.org/man/alarm.2.en) function, and when it is lonely, it doesn't like to be lonely so it [sleeps](https://man.archlinux.org/man/sleep.3.en) (it actually spawns a child process then exits, the child process is the one who sleeps but whatever). In both cases commands are executed using [system](https://man.archlinux.org/man/system.3).

## Default command

The default command for my use case is a bash script that beeps ([plays a file generated using ffmpeg](https://stackoverflow.com/questions/5109038/linux-sine-wave-audio-generator)) 3 times using mpv:

```bash
#! /usr/bin/bash

notify-send "time's up"

for i in {1..3}; do
  mpv ~/music/sine.wav
  sleep .3
done
```
