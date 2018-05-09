#ifndef GLOBALS_H
#define GLOBALS_H

#define SHARED_SIZE 10

#define THREAD_RUNNING 0 //thread is currently running
#define THREAD_READY 1 //thread is ready to be run
#define THREAD_SLEEPING 2 //set from call to thread_sleep()
#define THREAD_WAITING 3 //waiting on mutex or semaphore

struct system_t *sys;
struct semaphore_t *full;
struct semaphore_t *empty;
struct mutex_t *m;

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
#include "os.h"
#include "synchro.c"
#include "buffer.c"
#include "blink.c"
#include "stats.c"

#endif
