struct semaphore {
    int value;
    void *waitlist;
};

void sem_init(struct semaphore_t* s, int8_t value);

void sem_wait(struct semaphore_t* s);

void sem_signal(struct semaphore_t* s);

void sem_signal_swap(struct semaphore_t* s);