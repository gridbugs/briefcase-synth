#include <Wire.h>

#define BAUDRATE 9600
#define MCP4725_ADDR 0x60

#define CMD_UPDATE 64

void setup() {
  // Set up ADC
  analogReference(EXTERNAL);

  // Set up I2C which will be used to communicate with the DAC
  Wire.begin();

  // Set up pins that connect to scale switches
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  Serial.begin(BAUDRATE);
  Serial.write("Setup complete!\n\r");
}

int read_scale() {
  return digitalRead(8) | (digitalRead(9) << 1) | (digitalRead(10) << 2);
}

int chromatic_to_major(int note) {
  // tone, tone, semitone, tone, tone, tone, semitone
  switch (note % 12) {
    case 1:
    case 3:
    case 6:
    case 8:
    case 10:
      note -= 1;
      break;
  }
  return note;
}

int chromatic_to_pentatonic(int note) {
  switch (note % 12) {
    case 1:
    case 3:
    case 6:
    case 8:
    case 10:
      note -= 1;
      break;
    case 4:
    case 11:
      note -= 2;
  }
  return note;
}

int chromatic_to_octave(int note) {
  return (note / 12) * 12;
}

int chromatic_to_fifth_and_octave(int note) {
  int octave_base = (note / 12) * 12;
  int offset = note % 12;
  if (offset < 5) {
    return octave_base;
  } else {
    return octave_base + 5;
  }
}

int chromatic_to_selected_scale(int note) {
  switch (read_scale()) {
    case 0:
      return note;
    case 1:
      return chromatic_to_major(note);
    case 2:
      return chromatic_to_pentatonic(note);
    case 3:
      return chromatic_to_octave(note);
    case 4:
      return chromatic_to_fifth_and_octave(note);
  }
  return note;
}

void loop() {

  int input = analogRead(0);

  // Scale the input between 0 and 60 as there are 5 octaves made up of 12
  // semitones each. The 5 octaves comes from the fact that the maximum output
  // from the DAC is 5v, and that the synth uses a 1 volt-per-octave convention
  // for encoding frequencies. Divide by 1023 because that's the maximum value
  // that can be read from the ADC.
  double input_0_60 = ((double)input * 60.0) / 1023.0;

  // Round down to the nearest semitone.
  int input_0_60_quantized = chromatic_to_selected_scale((int)floor(input_0_60));

  // Convert to the range accepted by the DAC (12-bits) where 4095 is the
  // maximum value.
  int output = (int)(((double)input_0_60_quantized * 4095.0) / 60.0);

  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(CMD_UPDATE);
  Wire.write(output >> 4); // 8 most significant bits
  Wire.write((output & 15) << 4); // 4 least significant bits
  Wire.endTransmission();
  delay(1);
}
