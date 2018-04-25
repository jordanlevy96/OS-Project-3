typedef struct system_t {
    thread_t* array[2];
    int system_time;
    int current_thread; //id
} system_t;