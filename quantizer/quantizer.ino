#include <Wire.h>

#define BAUDRATE 9600
#define MCP4725_ADDR 0x60

#define CMD_UPDATE 64

void setup() {
  analogReference(EXTERNAL);
  Wire.begin();
  Serial.begin(BAUDRATE);
  Serial.write("Setup complete!\n\r");
}

void loop() {

  int input = analogRead(0);

  // scale between 0 and 60 as there are 5 octaves made up of 12 semitones each
  double input_0_60 = ((double)input * 60.0) / 1023.0;
  double input_0_60_quantized = floor(input_0_60);
  int output = (int)((input_0_60_quantized * 4095.0) / 60.0);

  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(CMD_UPDATE);
  Wire.write(output >> 4); // 8 most significant bits
  Wire.write((output & 15) << 4); // 4 least significant bits
  Wire.endTransmission();
  delay(1);
}
