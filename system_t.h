typedef struct system_t {
    thread_t* array[2];
    int num_threads;
    volatile uint8_t system_time;
    int current_thread; //id
} system_t;
