#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef OS_H
#define OS_H

//This structure defines the register order pushed to the stack on a
//system context switch.
typedef struct regs_context_switch {
   uint8_t padding; //stack pointer is pointing to 1 byte below the top of the stack

   //Registers that will be managed by the context switch function
   uint8_t r29;
   uint8_t r28;
   uint8_t r17;
   uint8_t r16;
   uint8_t r15;
   uint8_t r14;
   uint8_t r13;
   uint8_t r12;
   uint8_t r11;
   uint8_t r10;
   uint8_t r9;
   uint8_t r8;
   uint8_t r7;
   uint8_t r6;
   uint8_t r5;
   uint8_t r4;
   uint8_t r3;
   uint8_t r2;
   uint8_t eind;
   uint8_t pch;
   uint8_t pcl;
} regs_context_switch;

//This structure defines how registers are pushed to the stack when
//the system 10ms interrupt occurs.  This struct is never directly
//used, but instead be sure to account for the size of this struct
//when allocating initial stack space
typedef struct regs_interrupt {
   uint8_t padding; //stack pointer is pointing to 1 byte below the top of the stack

   //Registers that are pushed to the stack during an interrupt service routine
   uint8_t r31;
   uint8_t r30;
   uint8_t r27;
   uint8_t r26;
   uint8_t r25;
   uint8_t r24;
   uint8_t r23;
   uint8_t r22;
   uint8_t r21;
   uint8_t r20;
   uint8_t r19;
   uint8_t r18;
   uint8_t rampz; //rampz
   uint8_t sreg; //status register
   uint8_t r0;
   uint8_t r1;
   uint8_t eind;
   uint8_t pch;
   uint8_t pcl;
} regs_interrupt;

void start_system_timer(void);

void thread_start(void);

void context_switch(uint16_t* new_sp, uint16_t* old_sp);

void create_thread(char* name, uint16_t address, void* args, uint16_t stack_size);

int get_next_thread(void);

int get_thread_id(void);

struct process *get_current_process(void);

void thread_sleep(uint16_t ticks);

void main_thread(void);

void os_init(void);

void os_start(void);

#endif
