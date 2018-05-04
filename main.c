#include "os.c"

int main() {
    serial_init();
    os_init();
    os_start();

    return 0;
}