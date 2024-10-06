#include <stdio.h>
#include <avr/io.h>
#include "uart.h"

int main(void) {
    USART0_init();
    printf("Hello, World!\r\n");

    while (1) {
    }

    return 0;
}
