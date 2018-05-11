//Jordan Levy and Chris Moranda

typedef struct system_t {
    struct thread_t* array[NUM_THREADS];
    volatile uint16_t num_threads;
    volatile uint16_t system_time;
    volatile uint16_t current_thread; //id
    uint16_t producer_status;
} system_t;
