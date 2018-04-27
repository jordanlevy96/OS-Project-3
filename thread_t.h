typedef struct thread_t {
    uint16_t name;
    uint8_t id;
    uint16_t sp;
    uint16_t stack_size;
    uint16_t stack_base;
    // struct regs_context_switch registers;
} thread_t;