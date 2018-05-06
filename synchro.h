#ifndef "os.c"
#include "os.c"
#endif

struct mutex_t {
    int owner;
    int available;
    struct process *list; //waitlist buffer start
    int size; //buffer capacity
    int i; //buffer index
};

struct semaphore_t {
    int value;
    struct process *list; //waitlist buffer start
    int size; //buffer capacity
    int i; //buffer index
};

void mutex_init(struct mutex_t* m);

void mutex_lock(struct mutex_t* m);

void mutex_unlock(struct mutex_t* m);

void sem_init(struct semaphore_t* s, int8_t value);

void sem_wait(struct semaphore_t* s);

void sem_signal(struct semaphore_t* s);

void sem_signal_swap(struct semaphore_t* s);