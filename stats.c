//Jordan Levy and Chris Moranda

#include <avr/io.h>
#include <avr/interrupt.h>

void bogus_stats(struct system_t *sys) {
   struct thread_t *blink, *stats;
   uint16_t systime = sys->system_time;
   blink = sys->array[1];
   stats = sys->array[2];

   set_cursor(1, 1);
   print_string("System Time: ");
   print_int(systime / 100);

   set_cursor(2, 1);
   print_string("Number of Threads: ");
   print_int(sys->num_threads);

   // first thread data
   set_cursor(4, 1);
   print_string("~~~ Thread ");
   print_int(blink->id);
   print_string(" ~~~");

   set_cursor(5, 1);
   print_string("Thread Name: ");
   print_string(blink->name);

   // second thread data
   set_cursor(7, 1);
   print_string("~~~ Thread ");
   print_int(stats->id);
   print_string(" ~~~");

   set_cursor(8, 1);
   print_string("Thread Name: ");
   print_string(stats->name);

}

void stats(){

   register uint8_t foo asm ("r17");
   register uint8_t bar asm ("r16");

   uint16_t foobar = (((uint16_t) foo) << 8) | (uint16_t) bar;

   struct system_t *sys = (struct system_t *) foobar;

   uint16_t stack_end1, stack_end2;
   uint16_t size_used1, size_used2;
   uint16_t systime = sys->system_time;

   struct thread_t *thread1, *thread2;
   thread1 = sys->array[1];
   thread2 = sys->array[2];

   size_used1 = (thread1->stack_base - thread1->sp);
   stack_end1 = (thread1->stack_base - size_used1);

   size_used2 = (thread2->stack_base - thread2->sp);
   stack_end2 = (thread2->stack_base - size_used2);


   while (1) {
      set_cursor(1, 1);
      print_string("System time is: ");
      print_int(systime / 100);
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
      print_int("Current top of stack: ");
      print_hex(thread2->sp);
      set_cursor(19, 1);
      print_string("Stack base is: ");
      print_hex(thread2->stack_base);
      set_cursor(20, 1);
      print_string("End of stack at: ");
      print_hex(stack_end2);
   }
}
