//Jordan Levy and Chris Moranda
#include "os.h"

//This interrupt routine is automatically run every 10 milliseconds
ISR(TIMER0_COMPA_vect) {
    //At the beginning of this ISR, the registers r0, r1, and r18-31 have
    //already been pushed to the stack

    //The following statement tells GCC that it can use registers r18-r31
    //for this interrupt routine.  These registers (along with r0 and r1)
    //will automatically be pushed and popped by this interrupt routine.
    asm volatile ("" : : : "r18", "r19", "r20", "r21", "r22", "r23", "r24", \
                 "r25", "r26", "r27", "r30", "r31");

    //Insert your code here
    sys->system_time_ms++;

    //Call get_next_thread to get the thread id of the next thread to run
    int old = sys->current_thread;
    int next = get_next_thread();

    thread_t *old_thread = sys->array[old];
    thread_t *new_thread = sys->array[next];

    sys->current_thread = next;

    //Call context switch here to switch to that next thread
    context_switch(&new_thread->sp, &old_thread->sp);

    //At the end of this ISR, GCC generated code will pop r18-r31, r1,
    //and r0 before exiting the ISR
}

ISR(TIMER1_COMPA_vect) {
   //This interrupt routine is run once a second
   //The 2 interrupt routines will not interrupt each other

}

void start_system_timer() {
    //start timer 0 for OS system interrupt
    TIMSK0 |= _BV(OCIE0A);  //interrupt on compare match
    TCCR0A |= _BV(WGM01);   //clear timer on compare match

    //Generate timer interrupt every ~10 milliseconds
    TCCR0B |= _BV(CS02) | _BV(CS00);    //prescalar /1024
    OCR0A = 156;             //generate interrupt every 9.98 milliseconds

    //start timer 1 to generate interrupt every 1 second
    OCR1A = 15625;
    TIMSK1 |= _BV(OCIE1A);  //interrupt on compare
    TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10); //slowest prescalar /1024

}

//Call this to start the system timer interrupt
// void start_system_timer() {
//     TIMSK0 |= _BV(OCIE0A);  //interrupt on compare match
//     TCCR0A |= _BV(WGM01);   //clear timer on compare match

//     //Generate timer interrupt every ~10 milliseconds
//     TCCR0B |= _BV(CS02) | _BV(CS00) | _BV(CS02);    //prescalar /1024
//     OCR0A = 156;             //generate interrupt every 9.98 milliseconds
// }

// When you first create a thread, create the stack so that it runs thread_start.
// Then, thread_start() will go to the function you actually want to run in your
// thread. The function argument (args) that is passed to create_thread() will be
// moved to the correct registers by thread_start().  Once the argument is in
// the correct registers expected by the thread function, you can use the ijmp
// instruction to jump to the start of the thread function.
__attribute__((naked)) void thread_start(void) {
    sei(); //enable interrupts - leave as the first statement in thread_start()

    //set args to correct register for function call
    asm volatile ("movw r24, r4");

    //set Z register to address of thread function
    asm volatile ("movw r30, r2"); //was set to r2 and r3 in create_thread()
    asm volatile ("ijmp"); // jump to function

}

__attribute__((naked)) void context_switch(uint16_t* new_sp, uint16_t* old_sp) {
    //push manually saved registers
    asm volatile ("push r2");
    asm volatile ("push r3");
    asm volatile ("push r4");
    asm volatile ("push r5");
    asm volatile ("push r6");
    asm volatile ("push r7");
    asm volatile ("push r8");
    asm volatile ("push r9");
    asm volatile ("push r10");
    asm volatile ("push r11");
    asm volatile ("push r12");
    asm volatile ("push r13");
    asm volatile ("push r14");
    asm volatile ("push r15");
    asm volatile ("push r16");
    asm volatile ("push r17");
    asm volatile ("push r28");
    asm volatile ("push r29");

    //put hardware stack pointer in thread struct
    //put current SP in Z
    asm volatile ("clr r31"); //clear Z high byte
    asm volatile ("ldi r30, 0x5D"); //set Z low byte to location of SP

    //store current SP in r2 and r3
    asm volatile ("ld r2, Z+"); //low byte
    asm volatile ("ld r3, Z"); //high byte

    //load old_sp into Z
    asm volatile ("movw r30, r22");

    //set SP to thread struct pointer
    asm volatile ("st Z+, r2");
    asm volatile ("st Z, r3");

    //load new stack pointer into hardware
    asm volatile ("movw r30, r24"); // move new_sp to Z register
    asm volatile ("ld r2, Z+"); //load SP value into r2 & r3
    asm volatile ("ld r3, Z");

    asm volatile ("clr r31"); //clear Z high byte
    asm volatile ("ldi r30, 0x5D"); //set Z low byte to SP address
    asm volatile ("st Z+, r2"); //store new_sp (r2 & r3) as hardware SP
    asm volatile ("st Z, r3");

    //pop manually saved registers
    asm volatile ("pop r29");
    asm volatile ("pop r28");
    asm volatile ("pop r17");
    asm volatile ("pop r16");
    asm volatile ("pop r15");
    asm volatile ("pop r14");
    asm volatile ("pop r13");
    asm volatile ("pop r12");
    asm volatile ("pop r11");
    asm volatile ("pop r10");
    asm volatile ("pop r9");
    asm volatile ("pop r8");
    asm volatile ("pop r7");
    asm volatile ("pop r6");
    asm volatile ("pop r5");
    asm volatile ("pop r4");
    asm volatile ("pop r3");
    asm volatile ("pop r2");

    //return
    asm volatile("ret");
}

// Call this function once for each thread you want to create
// name - name of the thread (10 character maximum)
// address - address of the function for this thread
// args - pointer to function arguments
// stack_size - size of thread stack in bytes (does not include stack space to save registers)
void create_thread(char* name, uint16_t address, void* args, uint16_t stack_size) {
    int id = sys->num_threads++;
    struct thread_t *thread = (struct thread_t *) malloc(sizeof(struct thread_t));

    /* set up thread struct */
    thread->id = id;
    memcpy(thread->name, name, 10);
    thread->address = address;
    thread->stack_size = stack_size + sizeof(regs_context_switch)
        + sizeof(regs_interrupt);
    thread->stack_base = (uint16_t) malloc(sizeof(regs_context_switch)
        + sizeof(regs_interrupt) + stack_size); //bottom of stack, lowest address
    thread->sp = (uint16_t) thread->stack_base + stack_size
        + sizeof(regs_interrupt); //just enough space for the struct on the stack

    /* push first stack values */
    struct regs_context_switch *regs_struct = (regs_context_switch *) thread->sp;
    //set up PC bytes to pop to thread_start()
    regs_struct->eind = 0;
    regs_struct->pch = (uint8_t) (((uint16_t) thread_start & 0xFF00) >> 8);
    regs_struct->pcl = (uint8_t) ((uint16_t) thread_start & 0x00FF);

    //set function address to r2 and r3
    regs_struct->r3 = (uint8_t) ((address & 0xFF00) >> 8);
    regs_struct->r2 = (uint8_t) (address & 0x00FF);

    //set args pointer to r4 and r5
    regs_struct->r5 = (uint8_t) (((uint16_t) args & 0xFF00) >> 8);
    regs_struct->r4 = (uint8_t) ((uint16_t) args & 0x00FF);

    sys->array[id] = thread;
    sys->processes[id] = (struct process *) malloc(sizeof(struct process));
    struct process *p = sys->processes[id];
    p->status = THREAD_READY;
    p->id = id;
    memcpy(p->name, name, 10);
}

//return the id of the next thread to run
int get_next_thread(void) {
    int current = sys->current_thread;

    // print_string("old: ");
    // print_int(current);

    if (current == sys->num_threads) {
        current = 1;
    }
    else {
        current++;
    }

    // print_string(" new: ");
    // print_int(current);

    return current;
}

int get_thread_id(void) {
    return sys->current_thread;
}

struct process *get_current_process(void) {
    return sys->processes[get_thread_id()];
}

void thread_sleep(uint16_t ticks) {
    // set_sleep_mode(<mode>); //what mode???
    cli();
    // for (int i = 0; i < ticks; i++) {
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
    // }
}

void main_thread() {
    clear_screen();

    sys->current_thread = 2;

    start_system_timer();
    sei();

    context_switch(&sys->array[1]->sp, &sys->array[0]->sp);
}

//any OS specific initialization code
void os_init(void) {
    sys = (struct system_t *) malloc(sizeof(struct system_t));
    sys->num_threads = 1;
    sys->current_thread = 0;
    sys->system_time_ms = 0;
    sys->system_time_s = 0;

    //create main thread
    struct thread_t *main = (struct thread_t *) malloc(sizeof(struct thread_t));
    int main_stack_extra;
    main->id = 0;
    main->stack_base = (uint16_t) malloc(sizeof(regs_context_switch)
        + sizeof(regs_interrupt)
        + main_stack_extra); //bottom of stack, lowest address
    main->sp = main->stack_base + main_stack_extra + sizeof(regs_interrupt);
    //just enough space for the struct on the stack;
    sys->processes[0]->id = 0;
    sys->processes[0] = (struct process *) malloc(sizeof(struct process));
    sys->processes[0]->status = THREAD_READY;
    memcpy(sys->processes[0]->name, "main", 5);
}

//start running the OS
void os_start(void) {
    int delay = 500;

    void *shared_mem = malloc(SHARED_SIZE);

    create_thread("blink", (uint16_t) blink, 0, 25);
    create_thread("stats", (uint16_t) stats, sys, 200);
    create_thread("producer", (uint16_t) producer, shared_mem, 50);
    create_thread("consumer", (uint16_t) consumer, shared_mem, 50);

    main_thread();
}

