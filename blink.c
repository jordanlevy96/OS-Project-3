#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

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

void blink(){
   //TODO: make delay time an argument
   while(1){
      blinkOn();
      _delay_ms(50);
      blinkOff();
      _delay_ms(50);
   }
}

