#include <avr/io.h>
#include <avr/interrupt.h>

#include "os.h"
#include "serial.c"

#include "thread_t.h"
#include "system_t.h"

#include "blink.c"

void stats(struct system_t system, uint16_t systime){
   uint16_t stack_end;
   uint16_t size_used;
   struct thread_t thread;
   thread = system->array[system->current_thread];
   size_used = (thread->stack_base - thread->sp);
   stack_end = (thread->stack_base - size_used);
   set_cursor(1,1);
   print_string("System time is: ");
   print_int(systime);
   set_cursor(1,2);
   print_string("Number of threads in the system: ");
   print_int(system->num_thread);
   set_cursor(1,3);
   print_string("Thread id: ");
   print_int(thread->id);
   set_cursor(1,4);
   print_string("Thread name: ");
   print_string(thread->name);
   set_cursor(1,5);
   print_string("Starting PC of thread: ");
   print_hex(thread->address);
   set_cursor(1,6);
   print_string("Bytes used by the stack: ");
   print_int(size_used);
   set_cursor(1,7);
   print_string("Stack size: ");
   print_int(thread->stack_size); 
   set_cursor(1,8);
   print_int("Current top of stack: ");
   print_hex(thread->sp);
   set_cursor(1,9);
   print_string("Stack base is: ");
   print_hex(thread->stack_base);
   set_cursor(1,10);
   print_string("End of stack at: ");
   print_hex(stack_end);
}
