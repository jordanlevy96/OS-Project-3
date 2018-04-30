//Jordan Levy and Chris Moranda

#include <avr/io.h>
#include <avr/interrupt.h>

void stats(volatile struct system_t *s){
   volatile struct system_t* sys = s;
   uint16_t stack_end1, stack_end2;
   uint16_t size_used1, size_used2;

   struct thread_t *thread1, *thread2;
   thread1 = sys->array[1];
   thread2 = sys->array[2];

   size_used1 = (thread1->sp - thread1->stack_base);
   stack_end1 = (thread1->stack_base + size_used1);

   size_used2 = (thread2->sp - thread2->stack_base);
   stack_end2 = (thread2->stack_base + size_used2);


   while (1) {
      set_cursor(1, 1);
      print_string("System time is: ");
      print_int(sys->system_time / 100);
      set_cursor(2, 1);
      print_string("Number of threads in the system: ");
      print_int(sys->num_threads);

      set_cursor(4, 1);
      print_string("Thread id: ");
      print_int(thread1->id);
      set_cursor(5, 1);
      print_string("Thread name: ");
      print_string(thread1->name);
      set_cursor(6, 1);
      print_string("Starting PC of thread: ");
      print_hex(thread1->address);
      set_cursor(7, 1);
      print_string("Bytes used by the stack: ");
      print_int(size_used1);
      set_cursor(8, 1);
      print_string("Stack size: ");
      print_int(thread1->stack_size);
      set_cursor(9, 1);
      print_string("Current top of stack: ");
      print_hex(thread1->sp);
      set_cursor(10, 1);
      print_string("Stack base is: ");
      print_hex(thread1->stack_base);
      set_cursor(11, 1);
      print_string("End of stack at: ");
      print_hex(stack_end1);

      set_cursor(13, 1);
      print_string("Thread id: ");
      print_int(thread2->id);
      set_cursor(14, 1);
      print_string("Thread name: ");
      print_string(thread2->name);
      set_cursor(15, 1);
      print_string("Starting PC of thread: ");
      print_hex(thread2->address);
      set_cursor(16, 1);
      print_string("Bytes used by the stack: ");
      print_int(size_used2);
      set_cursor(17, 1);
      print_string("Stack size: ");
      print_int(thread2->stack_size);
      set_cursor(18, 1);
      print_string("Current top of stack: ");
      print_hex(thread2->sp);
      set_cursor(19, 1);
      print_string("Stack base is: ");
      print_hex(thread2->stack_base);
      set_cursor(20, 1);
      print_string("End of stack at: ");
      print_hex(stack_end2);
   }
}
