#include "synchro.h"

void mutex_init(struct mutex_t* m) {
    m->size = 2;
    m->list = calloc(m->size, sizeof(int));
    m->i = 0;
}

void mutex_lock(struct mutex_t* m) {
    if (!m->available) {
        m->list[m->i++] = get_thread_id();
        while (!m->available); //do nothing
    }

    m->available = 0;
    m->owner = get_thread_id();

}

void mutex_unlock(struct mutex_t* m) {
    m->available = 1;
    m->owner = m->list[m->id--];
}

void sem_init(struct semaphore_t* s, int8_t value) {
    s->value = value;
    s->list = calloc(s->value, sizeof(int));
    s->i = 0;
}

void sem_wait(struct semaphore_t* s) {
    s->value--;

    if (s->value < 0) {
        //add this process to s->list

        block();
    }
}

void sem_signal(struct semaphore_t* s) {
    //implementation!
}

void sem_signal_swap(struct semaphore_t* s) {
    //implementation!
}