#ifndef GLOBALS_H
#include "globals.h"
#endif

void consumer_animation(int i){
   int y = 1;
   int j;
   for(j = 1; j < 12; j++){
      set_cursor(j,20);
      write_byte('|');
      set_cursor(j,24);
      write_byte('|');
   }
   set_cursor(j-1,21);
   print_string("___");
   for(j = 10; j > (10-i); j--){
      set_cursor(j,22);
      write_byte('_');
   }
   delay_time(1000);
   set_cursor(j+1, 25);
   print_string("->");
   delay_time(1000);
   clear_screen();
   for(j = 1; j < 12; j++){
      set_cursor(j,20);
      write_byte('|');
      set_cursor(j,24);
      write_byte('|');
   }
   set_cursor(j-1,21);
   print_string("___");

   for(j = 10; j > (11-i); j--){
      set_cursor(j,22);
      write_byte('_');
   }
   set_cursor(j+2, 27);
   print_string("->X");

}

void producer_animation(int i){
   int y = 1;
   int j;
   for(j = 1; j < 12; j++){
      set_cursor(j,20);
      write_byte('|');
      set_cursor(j,24);
      write_byte('|');
   }
   set_cursor(j-1,21);
   print_string("___");
   for(j = 10; j > (10-i); j--){
      set_cursor(j,22);
      write_byte('_');
   }
   delay_time(1000);
   set_cursor(j+1, 25);
   print_string("->");
   delay_time(1000);
   clear_screen();
   for(j = 1; j < 12; j++){
      set_cursor(j,20);
      write_byte('|');
      set_cursor(j,24);
      write_byte('|');
   }
   set_cursor(j-1,21);
   print_string("___");

   for(j = 10; j > (11-i); j--){
      set_cursor(j,22);
      write_byte('_');
   }
   set_cursor(j+2, 27);
   print_string("->X");

}