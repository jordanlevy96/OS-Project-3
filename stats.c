#include <avr/io.h>
#include <avr/interrupt.h>

#include "os.h"
#include "serial.c"

#include "thread_t.h"
#include "system_t.h"

#include "blink.c"

void stats(void){
   while(1){
      set_cursor(1,1);
      print_hex(PC);
      set_cursor(1,3);
      print_int(stk_usg);
      set_cursor(1,5);
      print_int(stk_size);
      set_cursor(1,7);
      print_hex(stk_top);
      set_cursor(1,9);
      print_hex(stk_base);
      set_cursor(1,11);
      print_hex(stk_end);
   }
}
