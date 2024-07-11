#pragma once

#include "constants.h"

#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#define FILE_NAME "perf"
#define FILE_EXT ".out"

void init_timer(int num_print, int num_chunk);
void start_timer();
void stop_timer();
void stop_timer_message(char* message);
