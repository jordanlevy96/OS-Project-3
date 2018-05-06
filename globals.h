#ifndef GLOBALS_H
#define GLOBALS_H

#define SHARED_SIZE 10
struct system_t *sys;

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>

#include "serial.c"

#include "thread_t.h"
#include "system_t.h"
#include "process.h"
#include "os.h"
#include "synchro.c"
#include "buffer.c"
#include "blink.c"
#include "stats.c"

#endif