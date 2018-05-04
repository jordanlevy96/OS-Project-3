struct mutex {
    int owner;
    void *waitlist;
};

struct semaphore {
    int value;
    void *waitlist;
};

void mutex_init(struct mutex_t* m);

void mutex_lock(struct mutex_t* m);

void mutex_unlock(struct mutex_t* m);

void sem_init(struct semaphore_t* s, int8_t value);

void sem_wait(struct semaphore_t* s);

void sem_signal(struct semaphore_t* s);

void sem_signal_swap(struct semaphore_t* s);