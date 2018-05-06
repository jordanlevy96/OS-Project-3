#ifndef GLOBALS_H
#define GLOBALS_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <string.h>

#include "serial.c"

#include "thread_t.h"
#include "system_t.h"
#include "process.h"
#include "os.h"

#include "synchro.h"

#include "blink.c"
#include "stats.c"

#define SHARED_SIZE 10

struct system_t *sys;

#endif