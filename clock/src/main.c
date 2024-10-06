#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "uart.h"
#include "adc.h"

#define OUTPUT_PIN_PORTD 4
#define PERIOD_ADC_CHANNEL 0
#define DUTY_ADC_CHANNEL 1

#define SCALE_FACTOR_LOG2 10

int main(void) {
    ADC_init();
    USART0_init();

    DDRD |= (1 << OUTPUT_PIN_PORTD);

    while (1) {
        uint32_t period = ((uint32_t)ADC_read(PERIOD_ADC_CHANNEL)) << SCALE_FACTOR_LOG2;

        // shift by 10 to account for the 10-bit ADC
        uint32_t duty = (((uint32_t)ADC_read(DUTY_ADC_CHANNEL)) * period) >> 10;

        uint32_t i = 0;
        PORTD |= (1 << OUTPUT_PIN_PORTD);
        while (i < duty) {
            ++i;
        }
        PORTD &= ~(1 << OUTPUT_PIN_PORTD);
        while (i < period) {
            ++i;
        }
    }

    return 0;
}
