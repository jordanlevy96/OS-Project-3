#ifndef GLOBALS_H
#include "globals.h"
#endif

/*
    mutex_t *m, semaphore_t *full, and semaphore_t *empty are
    initialized in os_start
/*
    display on a different part of the screen (and in color) a nice,
        clear diagram showing the elements of the bounded buffer
    display the production rate in milliseconds (e.g. 1 item per 1000 ms)
    display the consumption rate in milliseconds (e.g. 1 item per 1000 ms)
        have the buffer size be 10
*/
void display_bounded_buffer(uint16_t shared_mem) {
    //implementation!
}

/*
    simulate producing an item and placing the item in the buffer
    initialize to 1 item per 1000 ms
    provide an animation to show when an item is placed in the buffer
*/
void producer(uint16_t shared_mem) {
    uint8_t *ptr = (uint8_t *) shared_mem;

    while (1) {
        thread_sleep(g_produce_speed);
        for (int i = 0; i < SHARED_SIZE; i++) {
            if (ptr[i] != 0) {
                //first empty space

                sem_wait(empty);

                //add something to the buffer
                mutex_lock(m);
                ptr[i] = 1;
                mutex_unlock(m);

                //turn on blink light
                sys->producer_status = 1;
                producer_animation(i);

                sem_signal(full);
                continue;
            }
        }

        //only gets here if buffer is full
        //turn off blink light
        sys->producer_status = 0;
        mutex_lock(m);
    }
}

/*
    simulate consuming an item and removing the item from the buffer
    initialize to 1 item per 1000 ms
    provide an animation to show when an item is removed from the buffer
*/
void consumer(uint16_t shared_mem) {
    uint8_t *ptr = (uint8_t *) shared_mem;

    while (1) {
        thread_sleep(g_consume_speed);
        for (int i = SHARED_SIZE - 1; i > 0; i--) {
            if (ptr[i] != 0) {
                //first empty space

                sem_wait(full);

                mutex_lock(m);
                ptr[i] = 0;
                mutex_unlock(m);

                consumer_animation(i);
                sem_signal(empty);
            }
        }

        //buffer is empty!
        mutex_lock(m);
    }
}