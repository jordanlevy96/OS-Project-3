#include "synchro.h"

void mutex_init(struct mutex_t* m) {
    m->size = NUM_THREADS;,
    m->list = calloc(m->size, sizeof(int));
    m->i = 0;
}

void mutex_lock(struct mutex_t* m) {
    if (!m->available) {
        struct process *p = get_current_process();
        m->list[m->i++] = p;
        p->status = THREAD_SLEEPING;
        sleep();
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
        struct process *p = get_current_process();
        p->status = THREAD_SLEEPING;
        s->list[s->id++] = p;
        sleep();
    }
}

//should change the first waiting thread to ready,
//but continue running in the current thread
void sem_signal(struct semaphore_t* s) {
    struct process *next = s->waitlist[s->id--];
    next->status = THREAD_READY;
}

//should immediately switch to the first waiting thread
//(do not wait for the next timer interrupt).
void sem_signal_swap(struct semaphore_t* s) {
    //implementation!
}