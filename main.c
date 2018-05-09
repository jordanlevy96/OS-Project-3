#include "os.c"

int main() {
    serial_init();
    /*os_init();
    os_start();*/
    clear_screen();
    producer_animation(8);
    delay_time(3000);
    consumer_animation(8);  
    return 0;
}
