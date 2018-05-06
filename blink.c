//Jordan Levy and Chris Moranda

#ifndef GLOBALS_H
#include "globals.h"
#endif

void blinkOn(){

   //DDRB |= 0x80;
   //PORTB |= 0x80;
   asm volatile("ldi r24, 0x24");
   asm volatile("ldi r25, 0x00");
   asm volatile("movw r30, r24");
   asm volatile("ld r18, Z");
   asm volatile("ldi r24, 0x24");
   asm volatile("ldi r25, 0x00");
   asm volatile("ori r18, 0x80");
   asm volatile("movw r30, r24");
   asm volatile("st Z, r18");
   asm volatile("ldi r24, 0x25");
   asm volatile("ldi r25, 0x00");
   asm volatile("movw r30, r24");
   asm volatile("ld r18, Z");
   asm volatile("ldi r24, 0x25");
   asm volatile("ldi r25, 0x00");
   asm volatile("ori r18, 0x80");
   asm volatile("movw r30, r24");
   asm volatile("st Z, r18");

}

void blinkOff(){

   /*DDRB |= 0x80;
   PORTB &= 0x4F;*/
   asm volatile("ldi r24, 0x24");
   asm volatile("ldi r25, 0x00");
   asm volatile("movw r30, r24");
   asm volatile("ld r18, Z");
   asm volatile("ldi r24, 0x24");
   asm volatile("ldi r25, 0x00");
   asm volatile("ori r18, 0x80");
   asm volatile("movw r30, r24");
   asm volatile("st Z, r18");
   asm volatile("ldi r24, 0x25");
   asm volatile("ldi r25, 0x00");
   asm volatile("movw r30, r24");
   asm volatile("ld r18, Z");
   asm volatile("ldi r24, 0x25");
   asm volatile("ldi r25, 0x00");
   asm volatile("andi r18, 0x4F");
   asm volatile("movw r30, r24");
   asm volatile("st Z, r18");


}

void delay_time(int time) {
   for (int i = 0; i < time; i++) {
      _delay_ms(1);
   }
}

void blink(void){
   int status;

   while(1){
      status = sys->producer_status;

      if (status)
         blinkOn();
      else
         blinkOff();
   }
}

