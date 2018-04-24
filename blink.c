#include <avr/io.h>
#include <stdio.h>
#include "globals.h"
#include "lab1_part1.c"
#include "lab1_part2.c"


void blink(){
   asm volatile("ldi r30, 0x24");
   asm volatile("ori r30, 0x80");
   asm volatile("st 0x24, Z");

   asm volatile("ld r30, 0x25");
   asm volatile("ori r30, 0x80");
   asm volatile("st 0x25, Z");

}

int main(void) {
   /*uint8_t t, u;
   serial_init();
   write_byte(27);
   print_string("[2J");
   name_cursor();*/
   blink();
   //uncomment this line to run part_b
   //run with make/ make program
   //increaseNumbers();
   return 0;
}
