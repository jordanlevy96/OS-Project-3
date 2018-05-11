#include "synchro.h"

void mutex_init(struct mutex_t* m) {
    m->size = NUM_THREADS;
    m->list = calloc(m->size, sizeof(struct thread_t *));
    m->i = 0;
}

void mutex_lock(struct mutex_t* m) {
    cli();
    if (!m->available) {
        struct thread_t *t = get_current_thread();
        m->list[m->i] = t;
        
        m->i++;
        //check for i overflow
        if (m->i >= m->size) {
            m->i = 0;
        }
        
        t->thread_state = THREAD_WAITING;
        switch_and_interrupt(t);
    }
    
    m->available = 0;
    m->owner = get_thread_id();
    sei();
}

void mutex_unlock(struct mutex_t* m) {
    cli();
    m->available = 1;
    m->i--;
    //check for i overflow
    if (m->i < 0) {
        m->i = m->size - 1;
    }
    
    struct thread_t *next = m->list[m->i];
    next->thread_state = THREAD_READY;
    
    m->owner = next->id;
    sei();
}

void sem_init(struct semaphore_t* s, int8_t value) {
    s->value = value;
    s->list = calloc(s->value, sizeof(int));
    s->i = 0;
}

void sem_wait(struct semaphore_t* s) {
    cli();
    s->value--;
    
    if (s->value < 0) {
        //add this process to s->list
        struct thread_t *t = get_current_thread();
        t->thread_state = THREAD_SLEEPING;
        s->i++;
        //check for i overflow
        if (s->i >= s->size) {
            s->i = 0;
        }
        s->list[s->i] = t;
        thread_sleep(10);
    }
    sei();
}

//should change the first waiting thread to ready,
//but continue running in the current thread
void sem_signal(struct semaphore_t* s) {
    cli();
    s->i--;
    //check for i overflow
    if (s->i < 0) {
        s->i = s->size;
    }
    struct thread_t *next = s->list[s->i];
    next->thread_state = THREAD_READY;
    sei();
}

//should immediately switch to the first waiting thread
//(do not wait for the next timer interrupt).
void sem_signal_swap(struct semaphore_t* s) {
    struct thread_t *next = s->list[s->i];
    struct thread_t *current = get_current_thread();
    
    context_switch(&next->sp, &current->sp);
}
