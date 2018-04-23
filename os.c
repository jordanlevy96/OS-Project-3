#include <avr/io.h>
#include <avr/interrupt.h>
#include "globals.h"
#include "os.h"

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
    //Call get_next_thread to get the thread id of the next thread to run
    //Call context switch here to switch to that next thread

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

__attribute__((naked)) void context_switch(uint16_t* new_sp, uint16_t* old_sp) {
    //push manually saved registers

    asm volatile("movw ZH:ZL, r23:r22;"); //put old sp into z
    // asm volatile("adiw ZH:ZL, -11;"); //increment z to register pointer
    asm volatile("st ZH:ZL, -11(ZH:ZL)"); //get location from pointer
    asm volatile("st 0x5E:0x5D, -1(ZH:ZL);"); //set SP to where we store r29

    //push register values onto stack
    asm volatile ("push r29;");
    asm volatile ("push r28;");
    asm volatile ("push r13;");
    asm volatile ("push r12;");
    asm volatile ("push r11;");
    asm volatile ("push r10;");
    asm volatile ("push r9;");
    asm volatile ("push r8");
    asm volatile ("push r7;");
    asm volatile ("push r6;");
    asm volatile ("push r5;");
    asm volatile ("push r4;");
    asm volatile ("push r3;");
    asm volatile ("push r2;");

    //put hardware stack pointer in thread struct
    asm volatile ("movw YH:YL, 0x5E:0x5D;"); //move hardware SP to Y register
    asm volatile ("movw ZH:ZL, r23:r22;"); //move thread SP to Z register
    asm volatile ("st 0x5E:0x5D, 2(ZH:ZL);"); //set hardware SP to thread SP
    asm volatile ("push Y;") //save thread SP onto thread stack

    //load new stack pointer into hardware
    asm volatile ("pop z, %0" : sys->array[get_next_thread()]->sp);
    asm volatile ("movw 0x5E, z");

    //pop manually saved registers
    asm volatile ("pop r29, %0");
    asm volatile ("pop r28, %0");
    asm volatile ("pop r13, %0");
    asm volatile ("pop r12, %0");
    asm volatile ("pop r11, %0");
    asm volatile ("pop r10, %0");
    asm volatile ("pop r9, %0");
    asm volatile ("pop r8, %0");
    asm volatile ("pop r7, %0");
    asm volatile ("pop r6, %0");
    asm volatile ("pop r5, %0");
    asm volatile ("pop r4, %0");
    asm volatile ("pop r3, %0");
    asm volatile ("pop r2, %0");

    //return
    asm volatile("ret");
}

__attribute__((naked)) void thread_start(void) {
    sei(); //enable interrupts - leave as the first statement in thread_start()
}

//any OS specific initialization code
void os_init(void) {
    sys = (struct system_t *) malloc(sizeof(struct system_t));
    sys->array[0] = (struct thread_t *) malloc(sizeof(struct thread_t));
    sys->array[1] = (struct thread_t *) malloc(sizeof(struct thread_t));

    // sys->array[0]->name = "blink";
    // sys->array[0]->id = 0;
    // sys->array[0]->sp = malloc(1000);
    // sys->array[0]->stack_size = 1000;

    // sys->array[1]->name = "stats";
    // sys->array[1]->id = 1;
    // sys->array[1]->sp = malloc(1000);
    // sys->array[1]->stack_size = 1000;

}

// Call this function once for each thread you want to create
// name - name of the thread (10 character maximum)
// address - address of the function for this thread
// args - pointer to function arguments
// stack_size - size of thread stack in bytes (does not include stack space to save registers)
void create_thread(char* name, uint16_t address, void* args, uint16_t stack_size) {
    //figure out id of new thread
    int id;

    struct thread_t current = sys->array[id];
    current->name = name;
    current->stack_size = stack_size;
    current->sp = malloc(stack_size + sizeof(regs_context_switch)
        + sizeof(regs_interrupt));

    struct regs_context_switch *context_struct = (struct regs_context_switch *)
        current->sp - 1;




}

//return the id of the next thread to run
uint8_t get_next_thread(void) {
    if (sys->current_thread == 0) {
        return 1;
    }
    else if (sys->current_thread == 1) {
        return 0;
    }
    else {
        return 0; //hopefully this never happens
    }
}

/*
//start running the OS
void os_start(void) {

}

// When you first create a thread, create the stack so that it runs thread_start.
// Then, thread_start() will go to the function you actually want to run in your
// thread. The function argument (args) that is passed to create_thread() will be
// moved to the correct registers by thread_start().  Once the argument is in
// the correct registers expected by the thread function, you can use the ijmp
// instruction to jump to the start of the thread function.

void thread_start(void) {

}

*/

int main() {
    os_init();
    // other stuff!
}

