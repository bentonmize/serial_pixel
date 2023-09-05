// Arduino Uno sketch for driving 12 LED NeoPixel ring with various animations
// 2023 Benton Mize

// Built on:
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define NEO_PIN   6
#define NUMPIXELS 12
#define BUFFER_SIZE 25

#define BRIGHTNESS 1
#define FILL 2

Adafruit_NeoPixel ring(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
byte buffer[BUFFER_SIZE];

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  ring.begin(); // init NeoPixel ring object (REQUIRED)

  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }

  Serial.println("Initialized!");
}

void parseBuffer(byte command[]) {
  int opcode = command[0];

  // For some reason, do not declare anything inside the switch
  switch (opcode)
  {
    case 1:
      ring.setBrightness(command[1]);
      Serial.print("Setting brightness!");
      break;
    case 2:
      Serial.println(command[1]);
      ring.fill(ring.Color(command[1], command[2], command[3]), 0, ring.numPixels());
      Serial.print("Setting color!");
      break;
    default:
      Serial.print("Unknown command!");
      break;
  }
}


void loop() {
  if(Serial.available() > 0) {
    int readBytes = Serial.readBytesUntil('\n', buffer, BUFFER_SIZE);
    if(readBytes > 0) {
      parseBuffer(buffer);
    }

    ring.show();
  }
}
