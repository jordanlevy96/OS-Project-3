#ifndef GLOBALS_H
#include "globals.h"
#endif

void display_buffer(){
   int j;
   for(j = 25; j > 14; j--){
      set_cursor(j,20);
      write_byte('|');
      set_cursor(j,24);
      write_byte('|');
   }
   set_cursor(25, 21);
   print_string("___"); //displays an empty buffer
}
void producer_animation(int i){
    int j;
    display_buffer();//creates empty buffer
    for(j = 25; j > (24-i); j--){
        set_cursor(j,22);//initializes i - 1 items in the buffer
                         //before producing the ith item
        write_byte('_');
    }
    // delay_time(200);
    set_cursor(24-i, 15);
    print_string("->x");//show item to right of buffer
    // delay_time(200);
    clear_screen();
    display_buffer();
    set_cursor(24-i, 15);
    print_string("->");//show item entering buffer
    for(j = 25; j > (23-i); j--){
        set_cursor(j,22);
        write_byte('_');//show new item in total items, totalling i items
    }
}

void consumer_animation(int i){
    int j;
    display_buffer();
    for(j = 25; j > (22-i); j--){
        set_cursor(j,22);//display i+1 items in buffer to start
        write_byte('_');
    }
    // delay_time(200);
    set_cursor(23-i, 25);
    print_string("->");
    // delay_time(200);
    clear_screen();
    display_buffer();
    for(j = 25; j > (23-i); j--){
        set_cursor(j,22);
        write_byte('_');//remove an item to leave i items in buffer
    }
    set_cursor(24-i, 25);
    print_string("->X");

}
