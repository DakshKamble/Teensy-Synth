#include <Arduino.h>
#include <SPI.h>

#define CS 2 //MCP4922 CS PIN

float phase = 0.0;
float freq = 200.0;
float sampleRate = 10000.0;

//For modulating main sine wave
float modMax = 400.0;
float modMin = 0.0;
float modPhase = 0.0;


//Function declarations
void writeDac(uint16_t value);
float getSineWave(float frequency);
float getModulator(float frequency);
float getSawtoothWave(float frequency);

void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  // 1. Get a value between -1.0 and 1.0 from the slow wave (0.5 Hz)
  float modValue = getModulator(0.5); 

  // 2. Map that -1 to 1 value to your 200 to 400 range
  // Middle is 300, spread is 100
  float targetFreq = 200.0 + (modValue * 100.0);

  // 3. Feed that "Target Frequency" into your main sine wave function
  //float finalSignal = getSineWave(targetFreq);
  float finalSignal = getSawtoothWave(targetFreq);

  // 4. Send to DAC
  writeDac((finalSignal + 1.0) * 2047);

  delayMicroseconds(100);
}





float getSawtoothWave(float frequency) {
  static float p = 0.0;
  float phaseInc = (2.0 * PI * frequency) / 10000.0;

  p += phaseInc;
  if(p > 2.0 * PI) p -= 2.0 * PI;

  float out = (p/PI) - 1.0;
  return out;

}

float getSineWave(float frequency) {
  static float phase = 0.0;
  float phaseInc = (2.0 * PI * frequency) / 10000.0;
  phase += phaseInc;
  if (phase > 2.0 * PI) phase -= 2.0 * PI;
  return sin(phase);
}

float getModulator(float frequency) {
  static float p_mod = 0.0; // Separate position for the modulator
  float phaseInc = (2.0 * PI * frequency) / 10000.0; 
  p_mod += phaseInc;
  if (p_mod > 2.0 * PI) p_mod -= 2.0 * PI;
  return sin(p_mod); 
}

void writeDac(uint16_t value) {
  uint16_t command = 0x3000 | (value & 0x0fff); //Define the 16bit packet for sending

  digitalWrite(CS, LOW); //Start transmission

  SPI.transfer16(command); //Send data packet

  digitalWrite(CS, HIGH); //pulling cs high for ending transmission
}