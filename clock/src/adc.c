#include <stdint.h>
#include <avr/io.h>

void ADC_init(void) {
    PRR &= ~(1 << PRADC); // disable power reduction ADC bit
    ADCSRA = (1 << ADEN); // enable the ADC
    DIDR0 = 0xFF; // enable all the ADC pins
    ADMUX = (1 << REFS0); // use supply voltage as reference voltage
}

void ADC_set_channel(uint8_t channel) {
    ADMUX = (1 << REFS0) | channel;
}

void ADC_start_read(void) {
    ADCSRA |= (1 << ADSC); // start the conversion
}

uint16_t ADC_complete_read(void) {
    while ((ADCSRA & (1 << ADSC)) != 0); // wait for the start bit to clear
    uint16_t lo = (uint16_t)ADCL;
    uint16_t hi = (uint16_t)ADCH << 8;;
    return hi | lo;
}

// Sample the ADC value on a channel.
uint16_t ADC_read_raw(uint8_t channel) {
    ADC_set_channel(channel);
    ADC_start_read();
    return ADC_complete_read();
}

// Sample the ADC value on a channel, discard the result, then sample it again.
// This seems to be necessary to prevent the value from the previous sample
// leaking into the result.
uint16_t ADC_read(uint8_t channel) {
    ADC_read_raw(channel);
    return ADC_read_raw(channel);
}
