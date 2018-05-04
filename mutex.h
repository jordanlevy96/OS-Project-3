struct mutex {
    int owner;
    void *waitlist;
};

void mutex_init(struct mutex_t* m);

void mutex_lock(struct mutex_t* m);

void mutex_unlock(struct mutex_t* m);