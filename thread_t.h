typedef struct {
    char *name;
    uint8_t id;
    uint16_t sp;
    uint16_t stack_size;
    struct regs_context_switch* registers;
} thread_t;