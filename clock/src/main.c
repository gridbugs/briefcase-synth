#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "uart.h"
#include "adc.h"

#define PERIOD_ADC_CHANNEL 0
#define DUTY_ADC_CHANNEL 1

#define SCALE_FACTOR_LOG2 10

#define BIT(n) (1 << n)

void io_port_init(void) {
    // input pins
    DDRB &= ~(BIT(0) | BIT(1) | BIT(2));
    // enable internal pull-up resistors
    PORTB |= (BIT(0) | BIT(1) | BIT(2));
    // output pins
    DDRD |= (BIT(6) | BIT(7));
    // clear output pins
    PORTD &= ~(BIT(6) | BIT(7));
}

void io_port_set_out_1(void) {
    PORTD |= BIT(6);
}

void io_port_set_out_2(void) {
    PORTD |= BIT(7);
}

void io_port_clear_out_1(void) {
    PORTD &= ~BIT(6);
}

void io_port_clear_out_2(void) {
    PORTD &= ~BIT(7);
}

int is_rand(void) {
    return (PINB & BIT(2)) != 0;
}

uint32_t speed_x(void) {
    switch (PINB & 0x3) {
        case 1:
            // bottom position
            return 0;
        case 3:
            // middle position (switch is floating so both bits are pulled high)
            return 1;
        case 2:
            // top position
            return 2;
        default:
            // should not happen but might because of analog reasons
            return 0;
    }
}

uint32_t period_10bit(void) {
    return (uint32_t)ADC_read(0);
}

uint32_t duty_1_10bit(void) {
    return (uint32_t)ADC_read(1);
}

uint32_t duty_2_10bit(void) {
    return (uint32_t)ADC_read(2);
}

uint32_t skip_10bit(void) {
    return (uint32_t)ADC_read(3);
}

uint32_t xorshift32_state = 0;

uint32_t xorshift32() {
    xorshift32_state ^= xorshift32_state << 13;
    xorshift32_state ^= xorshift32_state >> 17;
    xorshift32_state ^= xorshift32_state << 5;
    return xorshift32_state;
}

int main(void) {
    ADC_init();
    USART0_init();
    io_port_init();

    // seed the rng with the position of the skip dial multiplied by a large number
    xorshift32_state = skip_10bit() * 0x123456789;

    int count = 0;
    if (is_rand()) {
        count = xorshift32() & 0xF;
    }

    printf("Entering main loop...\n\r");
    while (1) {
#ifdef DEBUG_PRINT
        printf("rand: %d ", is_rand());
        printf("speed_x: %ld ", speed_x());
        printf("period: %ld ", period_10bit());
        printf("duty_1: %ld ", duty_1_10bit());
        printf("duty_2: %ld ", duty_2_10bit());
        printf("skip: %ld ", skip_10bit());
        printf("\n\r");
#endif

        uint32_t period = (period_10bit() << SCALE_FACTOR_LOG2) >> speed_x();
        uint32_t duty_1 = (duty_1_10bit() * period) >> 10;
        uint32_t duty_2 = (duty_2_10bit() * period) >> 10;
        io_port_set_out_1();
        int out_1_on = 1;
        int out_2_on = 0;
        // skip will be between 0 and 15
        int skip = (skip_10bit() >> 6);
        if (count >= skip) {
            io_port_set_out_2();
            out_2_on = 1;
            count = xorshift32() & 0xF;
        } else {
            if (is_rand()) {
                count = xorshift32() & 0xF;
            } else {
                count++;
            }
        }
        for (uint32_t i = 0; i < period; i++) {
            if (out_1_on && i > duty_1) {
                out_1_on = 0;
                io_port_clear_out_1();
            }
            if (out_2_on && i > duty_2) {
                out_2_on = 0;
                io_port_clear_out_2();
            }
        }
    }

    return 0;
}
