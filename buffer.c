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
void display_bounded_buffer() {
    //implementation!
}

void produce_animation(int size) {
    //print something fun
}

/*
    simulate producing an item and placing the item in the buffer
    initialize to 1 item per 1000 ms
    provide an animation to show when an item is placed in the buffer
*/
void producer(uint16_t shared_mem) {
    uint8_t *ptr = (uint8_t *) shared_mem;

    while (1) {
        for (int i = 0; i < SHARED_SIZE; i++) {
            if (ptr[i] != 0) {
                //first empty space

                sem_wait(empty);

                //add something to the buffer
                mutex_lock(m);
                ptr[i] = 1;
                mutex_unlock(m);

                //turn on blink light
                set_cursor(22, 1);
                print_string("producing!");
                sys->producer_status = 1;
                produce_animation(i);

                sem_signal(full);
                continue;
            }
        }

        //only gets here if buffer is full
        //turn off blink light
        set_cursor(22, 1);
        print_string("not producing!");
        sys->producer_status = 0;
        mutex_lock(m);
    }
}

void consume_animation(int size) {
    //print something fun
}

/*
    simulate consuming an item and removing the item from the buffer
    initialize to 1 item per 1000 ms
    provide an animation to show when an item is removed from the buffer
*/
void consumer(uint16_t shared_mem) {
    uint8_t *ptr = (uint8_t *) shared_mem;

    while (1) {
        for (int i = SHARED_SIZE - 1; i > 0; i--) {
            if (ptr[i] != 0) {
                //first empty space
                set_cursor(23, 1);
                print_string("consuming!");

                sem_wait(full);

                mutex_lock(m);
                ptr[i] = 0;
                mutex_unlock(m);

                consume_animation(i);
                sem_signal(empty);
            }
        }

        //buffer is empty!
        set_cursor(23, 1);
        print_string("not consuming!");
        mutex_lock(m);
    }
}