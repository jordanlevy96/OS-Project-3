#include <avr/io.h>
#include <avr/interrupt.h>

#include "os.h"
#include "serial.c"

#include "thread_t.h"
#include "system_t.h"

#include "blink.c"

#define STACKSIZE sizeof(regs_context_switch) + sizeof(regs_interrupt) + 100

int time;
time = 0;

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

    set_cursor(30, 1);
    print_string("interrupt: ");
    print_int(sys->system_time);

    sys->system_time++;

    //Call get_next_thread to get the thread id of the next thread to run
    int next = get_next_thread();

    thread_t *old_thread = sys->array[sys->current_thread];
    thread_t *new_thread = sys->array[next]; 

    sys->current_thread = next;

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

    print_string("thread starting!");
    //set Z register to address of thread function
    // asm volatile ("movw Z, Y"); //move function address to Z

    asm volatile ("movw Z, Y");
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

    //store current SP in X
    asm volatile ("ld r26, Z+"); //low byte
    asm volatile ("ld r27, Z"); //high byte

    //load thread sp address into Z
    asm volatile ("movw r30, r22");

    asm volatile ("st Z+, r26"); //set SP to thread struct pointer
    asm volatile ("st Z, r27");


    //load new stack pointer into hardware
    asm volatile ("movw r30, r24"); // move new_sp to Z register
    asm volatile ("ld r28, Z+"); //load SP value into r28 & 29
    asm volatile ("ld r29, Z");

    asm volatile ("clr r27"); //clear X high byte
    asm volatile ("ldi r26, 0x5D"); //set X low byte to SP address
    asm volatile ("st X+, r28"); //store new_sp (Y) as hardware SP
    asm volatile ("st X, r29");

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
    sys->num_threads = 0;
    sys->array[0] = (struct thread_t *) malloc(sizeof(struct thread_t));
    sys->array[1] = (struct thread_t *) malloc(sizeof(struct thread_t));

}

// Call this function once for each thread you want to create
// name - name of the thread (10 character maximum)
// address - address of the function for this thread
// args - pointer to function arguments
// stack_size - size of thread stack in bytes (does not include stack space to save registers)
void create_thread(char* name, uint16_t address, void* args, uint16_t stack_size) {
    //figure out id of new thread
    int id = sys->num_threads;
    sys->num_threads++;

    set_cursor(2 + (id * 10), 1);
    print_string("creating thread ");
    print_int(id);

    struct thread_t *current = sys->array[id];
    current->name = (uint16_t) name;
    current->stack_size = stack_size;
    current->stack_base = (uint16_t) malloc(stack_size);
    current->sp = current->stack_base;
    current->address = address;

    set_cursor(3 + (id * 10), 1);
    print_string("stack base ");
    print_hex(current->stack_base);

    struct regs_context_switch *context_struct = (struct regs_context_switch *)
        current->sp;

    set_cursor(4 + (id * 10), 1);
    print_string("context struct ");
    print_hex(context_struct);

    //set function address to registers 28 and 29
    context_struct->r29 = (address) >> 8;
    context_struct->r28 = (address) & 0xFF;

    //set arguments in these registers
    context_struct->r17 = *(uint8_t *)args; //overflow down into others

    context_struct->eind = 0;
    context_struct->pch = ((uint16_t) thread_start) >> 8;
    context_struct->pcl = ((uint16_t) thread_start) & 0xFF;

    set_cursor(5 + (id * 10), 1);
    print_string("pch ");
    print_hex(&context_struct->pch);
    print_string("\n");

    set_cursor(6 + (id * 10), 1);
    print_string("pcl ");
    print_hex(&context_struct->pcl);
    print_string("\n");

    if (id == 0) {
        current->sp = &context_struct->r29;
    }
    else if (id == 1) {
        current->sp = &context_struct->r2;
    }

    set_cursor(7 + (id * 10), 1);
    print_string("final sp ");
    print_hex(current->sp);
}

//return the id of the next thread to run
int get_next_thread(void) {
    if (sys->current_thread == sys->num_threads) {
        return 0;
    }
    else {
        return sys->current_thread + 1;
    }
}

void test() {
    set_cursor(25, 1);
    print_string("test prints!");
}

//start running the OS
void os_start(void) {
    print_string("os start\n");

    sys->system_time = 0;
    start_system_timer();

    create_thread("test", test, 0, STACKSIZE);
    create_thread("blink", blink, 0, STACKSIZE);

    // print_string("thread 1 is ");
    // print_string(sys->array[1]->name);

    set_cursor(10, 1);
    print_string("calling context_switch");

    context_switch(&sys->array[0]->sp, &sys->array[1]->sp);

    set_cursor(15, 1);
    print_string("end of os start!\n");
}


int main() {
    serial_init();
    // clear_screen();
    // print_string("main!\n");
    os_init();
    os_start();
    // other stuff!
    return 0;
}

