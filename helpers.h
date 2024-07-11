#pragma once

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>

#define OPTIONS "p:c:"

int argparser(int argc, char **argv, int* num_print, int* num_chunks);
