//Jordan Levy and Chris Moranda

#include <avr/io.h>
#include <avr/interrupt.h>

#include "os.h"
#include "serial.c"

#include "thread_t.h"
#include "system_t.h"

#include "blink.c"
#include "stats.c"

#define STACKSIZE sizeof(regs_context_switch) + sizeof(regs_interrupt) + 300

struct system_t *sys;

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
    sys->system_time++;

    //Call get_next_thread to get the thread id of the next thread to run
    int old = sys->current_thread;
    int next = get_next_thread();

    thread_t *old_thread = sys->array[old];
    thread_t *new_thread = sys->array[next];

    sys->current_thread = next;

    set_cursor(5, 1);
    print_string("interrupt ");
    print_int(sys->system_time);

    //Call context switch here to switch to that next thread
    context_switch(&new_thread->sp, &old_thread->sp);

    //At the end of this ISR, GCC generated code will pop r18-r31, r1,
    //and r0 before exiting the ISR
}

//Call this to start the system timer interrupt
void start_system_timer() {
    TIMSK0 |= _BV(OCIE0A);  //interrupt on compare match
    TCCR0A |= _BV(WGM01);   //clear timer on compare match

    //Generate timer interrupt every ~10 milliseconds
    TCCR0B |= _BV(CS02) | _BV(CS00) | _BV(CS02);    //prescalar /1024
    OCR0A = 156;             //generate interrupt every 9.98 milliseconds
}

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

    //load pointer to thread sp into Z
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

//any OS specific initialization code
void os_init(void) {
    sys = (struct system_t *) malloc(sizeof(struct system_t));
    sys->num_threads = 1;
    sys->current_thread = 0;
    sys->system_time = 0;
    sys->array[0] = (struct thread_t *) malloc(sizeof(struct thread_t));
    sys->array[1] = (struct thread_t *) malloc(sizeof(struct thread_t));
    sys->array[2] = (struct thread_t *) malloc(sizeof(struct thread_t));

    set_cursor(1, 1);
    print_string("os initialized");
}

// Call this function once for each thread you want to create
// name - name of the thread (10 character maximum)
// address - address of the function for this thread
// args - pointer to function arguments
// stack_size - size of thread stack in bytes (does not include stack space to save registers)
void create_thread(char* name, uint16_t address, void* args, uint16_t stack_size) {
    int id = get_next_thread();
    struct thread_t *thread = sys->array[id];

    /* set up thread struct */
    thread->id = id;
    thread->name = (uint16_t) name;
    thread->address = address;
    thread->stack_size = stack_size;
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
    regs_struct->r2 = (uint8_t) ((address & 0xFF00) >> 8);
    regs_struct->r3 = (uint8_t) (address & 0x00FF);

    //set args pointer to r4 and r5
    regs_struct->r4 = (uint8_t) (((uint16_t) args & 0xFF00) >> 8);
    regs_struct->r5 = (uint8_t) ((uint16_t) args & 0x00FF);

    sys->num_threads++;
}

//return the id of the next thread to run
int get_next_thread(void) {
    int current = sys->current_thread;

    if (current == 0 || current == 2) {
        current = 1;
    }
    else {
        current = 2;
    }

    return current;
}

void test() {
    set_cursor(25, 1);
    print_string("test prints!");
}

void main_thread() {
    create_thread("blink", blink, 0, STACKSIZE);
    create_thread("stats", stats, sys, STACKSIZE);

    set_cursor(3, 1);
    print_string("calling context_switch...");

    clear_screen();

    start_system_timer();
    sei();

    sys->current_thread = 2;

    context_switch(&sys->array[2]->sp, &sys->array[0]->sp);
}

//start running the OS
void os_start(void) {
    set_cursor(2, 1);
    print_string("os start\n");

    int main_size = STACKSIZE;

    struct thread_t *main = sys->array[0];
    main->stack_base = (uint16_t) malloc(main_size); //make a main stack
    main->sp = main->stack_base + main_size + sizeof(regs_interrupt);
    //just enough space for the struct on the stack;


    main_thread();
}


int main() {
    serial_init();
    os_init();
    clear_screen();
    os_start();

    return 0;
}

