#include <Wire.h>
#include <MIDI.h>

#define MCP4725_ADDR 0x60
#define CMD_UPDATE 64

#define PIN_LED_GREEN 6
#define PIN_LED_RED 7
#define LED_DURATION_TICKS 400

#define PIN_GATE 8

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

typedef struct {
    int pin;
    int ticks_remaining;
} led_handler_t;

led_handler_t led_handler_init(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    return {
        .pin = pin,
        .ticks_remaining = 0,
    };
}

void led_handler_flash(led_handler_t *led_handler) {
    digitalWrite(led_handler->pin, LOW);
    led_handler->ticks_remaining = LED_DURATION_TICKS;
}

void led_handler_tick(led_handler_t *led_handler) {
    led_handler->ticks_remaining -= 1;
    if (led_handler->ticks_remaining <= 0) {
        digitalWrite(led_handler->pin, HIGH);
    }
}

led_handler_t led_green;
led_handler_t led_red;

void setDAC(int value) {
    Wire.beginTransmission(MCP4725_ADDR);
    Wire.write(CMD_UPDATE);
    Wire.write(value >> 4); // 8 most significant bits
    Wire.write((value & 15) << 4); // 4 least significant bits
    Wire.endTransmission();
}

int midiNoteToDACInput(byte noteIndex) {
    #define DAC_MAX 4095L
    #define OUT_VOLTAGE_MAX 5L
    #define NOTES_PER_OCTAVE 12L
    return (int)((((long)noteIndex) * DAC_MAX) / (OUT_VOLTAGE_MAX * NOTES_PER_OCTAVE));
}

void setup() {
    led_green = led_handler_init(PIN_LED_GREEN);
    led_red = led_handler_init(PIN_LED_RED);

    led_handler_flash(&led_green);
    led_handler_flash(&led_red);

    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();

    pinMode(PIN_GATE, OUTPUT);
    digitalWrite(PIN_GATE, LOW);

    // Set up I2C which will be used to communicate with the DAC
    Wire.begin();
}

void loop() {
    static int gateCount = 0;
    if (MIDI.read()) {
        byte messageType = MIDI.getType();
        if (messageType == midi::NoteOn) {
            led_handler_flash(&led_green);
            byte noteIndex = MIDI.getData1();
            setDAC(midiNoteToDACInput(noteIndex));
            gateCount += 1;
        } else if (messageType == midi::NoteOff) {
            led_handler_flash(&led_red);
            gateCount -= 1;
        }

        if (gateCount > 0) {
            digitalWrite(PIN_GATE, HIGH);
        } else {
            digitalWrite(PIN_GATE, LOW);
        }
    }

    led_handler_tick(&led_green);
    led_handler_tick(&led_red);
}
