//Jordan Levy and Chris Moranda

#define NUM_THREADS 8

typedef struct system_t {
    struct thread_t* array[NUM_THREADS];
    struct process* processes[NUM_THREADS];
    int num_threads;
    volatile uint16_t system_time_ms;
    volatile uint16_t system_time_s;
    volatile int current_thread; //id
} system_t;
