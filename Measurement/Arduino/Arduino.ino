// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"

const int ledPin = 5;
const int refPin = 9;    // reference output

long startMicros = 0;
volatile long endMicros = 0;
volatile char detected = 0;

#define MICROSPERSECOND 1000000
long timeOut = MICROSPERSECOND * 5;

void setup() {
  // init serial port
  Serial.begin(9600);
  Serial.println("* Latency measurement tool *");

  pinMode(refPin, OUTPUT);   // sets the pin as output
  analogWrite(refPin, 128);
  pinMode(ledPin, OUTPUT);

  // setup comparator
  analogComparator.setOn(AIN0, AIN1); // sensor: D6, reference: D7 on Arduino Uno

  analogComparator.enableInterrupt(pulseDetected, RISING); // interrupt on rising edge
}

void loop() {
  if (Serial.available() > 0) {
    processUI();
  }
}

void measure() {
  Serial.println("Starting measurement...");
  digitalWrite(ledPin, HIGH);
  startMicros = micros();
  delay(100);
  digitalWrite(ledPin, LOW);

  //wait for detection
  while (!detected) {
    long time = micros() - startMicros;
    if (time > timeOut) break;
    delay(1000);
  }

  if (detected) {
    long latency = endMicros - startMicros;

    //print delay on console
    Serial.print("Detected: ");
    Serial.println(latency);

    //reset flag
    detected = 0;
  }
  else {
    Serial.println("Timed out. Check placement");
  }
}

void pulseDetected() {
  endMicros = micros();
  detected = 1;
}

void processUI() {
  char inchar = Serial.peek();
  switch (inchar) {
    case ('m'): measure();
      break;
    case ('\n'): void(); // ignore
      break;

    default: setRefValue();
  }

  // Flush buffer
  while(Serial.available() > 0) {
    Serial.read();
  }
}

void setRefValue() {
  int value = Serial.parseInt();
  if (value > 0) {
    analogWrite(refPin, value);
    Serial.print("New reference set to: ");
    Serial.println(value);
    delay(1000); // wait for RC filter to stabilize
  }
  else {
    Serial.println("Unknown command");
  }
}


