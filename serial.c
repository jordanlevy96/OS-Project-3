#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>

/*
 * Initialize the serial port.
 */
void serial_init() {
   uint16_t baud_setting;

   UCSR0A = _BV(U2X0);
   baud_setting = 16; //115200 baud

   // assign the baud_setting
   UBRR0H = baud_setting >> 8;
   UBRR0L = baud_setting;

   // enable transmit and receive
   UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
}

/*
 * Return 1 if a character is available else return 0.
 */
uint8_t byte_available() {
   return (UCSR0A & (1 << RXC0)) ? 1 : 0;
}

/*
 * Unbuffered read
 * Return 255 if no character is available otherwise return available character.
 */
uint8_t read_byte() {
   if (UCSR0A & (1 << RXC0)) return UDR0;
   return 255;
}

/*
 * Unbuffered write
 *
 * b byte to write.
 */
uint8_t write_byte(uint8_t b) {
   //loop until the send buffer is empty
   while (((1 << UDRIE0) & UCSR0B) || !(UCSR0A & (1 << UDRE0))) {}

   //write out the byte
   UDR0 = b;
   return 1;
}

//print a string
void print_string(char* s) {
   char c = *s;

   while (c != '\0') {
      write_byte(c);
      c = *++s;
   }
}

// print an 8-bit or 16-bit unsigned integer
void print_int(uint16_t i) {
   char s[6], c;
   int flag = 0;

   s[0] = '0' + (i/10000 % 10);
   s[1] = '0' + (i/1000 % 10);
   s[2] = '0' + (i/100 % 10);
   s[3] = '0' + (i/10 % 10);
   s[4] = '0' + (i % 10);
   s[5] = '\0';

   for (int i = 0; i < 5; i++) {
      c = s[i];
      if (!flag && c == '0') {
         continue;
      }
      else {
         flag = 1;
         print_string(&s[i]);
         return;
      }
   }

   print_string("0");
}

// print a 32-bit unsigned integer
void print_int32(uint32_t i) {
   char s[10], c;
   int flag = 0;

   s[0] = '0' + (i/1000000000 % 10);
   s[1] = '0' + (i/100000000 % 10);
   s[2] = '0' + (i/10000000 % 10);
   s[3] = '0' + (i/1000000 % 10);
   s[4] = '0' + (i/100000 % 10);
   s[5] = '0' + (i/10000 % 10);
   s[6] = '0' + (i/1000 % 10);
   s[7] = '0' + (i/100 % 10);
   s[8] = '0' + (i/10 % 10);
   s[9] = '0' + (i % 10);
   s[10] = '\0';

   for (int i = 0; i < 10; i++) {
      c = s[i];
      if (!flag && c == '0') {
         continue;
      }
      else {
         flag = 1;
         print_string(&s[i]);
         return;
      }
   }

   print_string("0");
}

static char hex [] = {'0', '1', '2', '3', '4', '5', '6',
            '7', '8', '9' ,'A', 'B', 'C', 'D', 'E', 'F' };

//print an 8-bit or 16-bit unsigned integer in hex format
void print_hex(uint16_t i) {
   int len = 0;
   char *str = malloc(4);

   while (i) {
        //get the equivalent hex digit
        str[len] = hex[i & 0xF];
        len++;
        i >>= 4; //shift i down 4 bits for next hex char
    }

   //since we get the digits in the wrong order, reverse the digits in the str
   for(int j = 0; j < len/2; j++) {
      str[j] ^= str[len - j - 1];
      str[len - j - 1] ^= str[j];
      str[j] ^= str[len - j - 1];
   }
   //null terminate the string and print
   str[len]='\0';
   print_string("0x");
   print_string(str);
   free(str);
}

// print a 32-bit unsigned integer in hex format
// same as 8- or 16-bit but with a bigger str buffer
void print_hex32(uint32_t i) {
   int len = 0;
   char *str = malloc(8);

   while (i) {
        //get the equivalent hex digit
        str[len] = hex[i & 0xF];
        len++;
        i >>= 4; //shift i down 4 bits for next hex char
    }

   //since we get the digits in the wrong order, reverse the digits in the str
   for(int j = 0; j < len/2; j++) {
      str[j] ^= str[len - j - 1];
      str[len - j - 1] ^= str[j];
      str[j] ^= str[len - j - 1];
   }
   //null terminate the string and print
   str[len]='\0';
   print_string("0x");
   print_string(str);
   free(str);
}

// clear output screen
void clear_screen() {
   write_byte(27);
   print_string("[2J");
}

// set the cursor position
void set_cursor(uint8_t row, uint8_t col) {
   write_byte(27);
   print_string("[");
   print_int(row);
   print_string(";");
   print_int(col);
   print_string("H");

}

// set the foreground color
void set_color(uint8_t color) {
   write_byte(27);
   print_string("[");
   print_int(color);
   print_string("m");

}

