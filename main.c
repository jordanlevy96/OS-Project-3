#include <stdio.h>

int main() {
    printf("hello, world!\n");
}

/*
//any OS specific initialization code
void os_init(void) {

}

// Call this function once for each thread you want to create
// name - name of the thread (10 character maximum)
// address - address of the function for this thread
// args - pointer to function arguments
// stack_size - size of thread stack in bytes (does not include stack space to save registers)
void create_thread(char* name, uint16_t address, void* args, uint16_t stack_size) {

}

//start running the OS
void os_start(void) {

}

//return the id of the next thread to run
uint8_t get_next_thread(void) {

}

// When you first create a thread, create the stack so that it runs thread_start.
// Then, thread_start() will go to the function you actually want to run in your
// thread. The function argument (args) that is passed to create_thread() will be
// moved to the correct registers by thread_start().  Once the argument is in
// the correct registers expected by the thread function, you can use the ijmp
// instruction to jump to the start of the thread function.

void thread_start(void) {

}

*/