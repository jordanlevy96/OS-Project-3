#ifndef GLOBALS_H
#include "globals.h"
#endif

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
                ptr[i] = 1;
                //turn on blink light
                sys->producer_status = 1;
                produce_animation(i);
                continue;
            }
        }

        //only gets here if buffer is full
        //turn off blink light
        thread_sleep(10);
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

    for (int i = SHARED_SIZE - 1; i > 0; i--) {
        if (ptr[i] != 0) {
            //first empty space
            ptr[i] = 0;
            produce_animation(i);
            return;
        }
    }
}