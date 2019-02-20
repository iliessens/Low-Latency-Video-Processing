// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"

// Should fast library be used?
#define USEFAST 0

#if USEFAST
// Faster I/O library from https://github.com/NicksonYap/digitalWriteFast
#include <digitalWriteFast.h>
#endif

int state = 0;
const int outPin = 5;
const int refPin = 9;   // reference output

void setup() {
  pinMode(refPin, OUTPUT);   // sets the pin as output
  analogWrite(refPin, 128);

#if USEFAST
  pinModeFast(outPin, OUTPUT);
#else
  pinMode(outPin, OUTPUT);
#endif
  // setup comparator
  analogComparator.setOn(AIN0, AIN1); // sensor: D6, reference: D7 on Arduino Uno

  analogComparator.enableInterrupt(edgeDetected, CHANGE); // interrupt on rising edge
}

void edgeDetected() {
  if (state == 1) { // zo geschreven omdat bibliotheek constanten nodig heeft
#if USEFAST
    digitalWriteFast(outPin, LOW);
#else
    digitalWrite(outPin, LOW);
#endif
  }
  else {
#if USEFAST
    digitalWriteFast(outPin, HIGH);
#else
    digitalWrite(outPin, HIGH);
#endif
  }
  state = ! state;
}

void loop() {
  // emtpy
}
