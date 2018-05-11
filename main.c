#include "os.c"

int main() {
    int i;
    serial_init();
    //os_init();
    //os_start();
    clear_screen();
    /*while(1){
       for(i = 9; i > 0; i--){
          consumer_animation(i);
       } 
       for(i = 0; i < 10; i++){
          producer_animation(i);
       }
    }*/
    display_bounded_buffer(8);
    return 0;

}
