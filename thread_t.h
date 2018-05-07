//Jordan Levy and Chris Moranda

typedef struct thread_t {
    char name[10];
    uint8_t id;
    uint16_t sp;
    uint16_t stack_size;
    uint16_t stack_base;
    uint16_t address;
    uint8_t thread_state;
    uint16_t sleep_timer;
} thread_t;
