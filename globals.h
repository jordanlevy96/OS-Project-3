//Global defines

#ifndef GLOBALS_H
#define GLOBALS_H

#define Red 31
#define Green 32
#define Yellow 33
#define Blue 34
#define Magenta 35
#define Cyan 36
#define White 37

//place defines and prototypes here
void print_string(char* s);

void print_int(uint16_t i);

void print_int32(uint32_t i);

void print_hex(uint16_t i);

void print_hex32(uint32_t i);

void set_cursor(uint8_t row, uint8_t col);

void set_color(uint8_t color);

void clear_screen(void);


#endif
