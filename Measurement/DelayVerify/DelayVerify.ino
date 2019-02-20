// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"
// Faster I/O library from https://github.com/NicksonYap/digitalWriteFast
#include <digitalWriteFast.h>

int state = 0;
const int outPin = 5;
const int refPin = 9;   // reference output

void setup() {
  pinMode(refPin, OUTPUT);   // sets the pin as output
  analogWrite(refPin, 128);

  pinModeFast(outPin, OUTPUT);
  // setup comparator
  analogComparator.setOn(AIN0, AIN1); // sensor: D6, reference: D7 on Arduino Uno

  analogComparator.enableInterrupt(edgeDetected, CHANGE); // interrupt on rising edge
}

void edgeDetected() {
  if (state == 1) { // zo geschreven omdat bibliotheek constanten nodig heeft
    digitalWriteFast(outPin, LOW);
  }
  else {
    digitalWriteFast(outPin, HIGH);
  }
  state = ! state;
}

void loop() {
  // emtpy
}
