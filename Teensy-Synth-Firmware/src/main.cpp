#include <Arduino.h>
#include <SPI.h>

#define CS 2 //MCP4922 CS PIN

float phase = 0.0;
float freq = 200.0;
float sampleRate = 10000.0;

//Function declarations
void writeDac(uint16_t value);

void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  phase += 0.05; //Controls speed
  if (phase > 2 * PI) {
  phase -= 2 * PI;
  }
  float s = sin(phase);
  uint16_t value = (s + 1)* 2047; 

  writeDac(value);
  delayMicroseconds(100);
}






void writeDac(uint16_t value) {
  uint16_t command = 0x3000 | (value & 0x0fff); //Define the 16bit packet for sending

  digitalWrite(CS, LOW); //Start transmission

  SPI.transfer16(command); //Send data packet

  digitalWrite(CS, HIGH); //pulling cs high for ending transmission
}