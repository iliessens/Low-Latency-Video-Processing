#include "analogComp.h"

const int refPin = 9;    // reference output

long startMicros = 0;
volatile long endMicros = 0;
volatile char detected = 0;

char error = 0;

void setup() {
  // init serial port
  Serial.begin(9600);
  Serial.println("* Verification tool *");

  //set reference. Alternative: voltage divider
  pinMode(refPin, OUTPUT);   // sets the pin as output
  analogWrite(refPin, 128);

  analogComparator.setOn(AIN0, AIN1); // square wave: D6, reference: D7 on Arduino Uno

  analogComparator.enableInterrupt(pulseDetected, RISING); // interrupt on rising edge

}

void loop() {
  if (error) {
    Serial.println("Error detected.");
    error = 0;
  }
  else if (detected) {
    char firstRun = 0;
    if (startMicros == 0) firstRun = 1;

    long period = endMicros - startMicros;
    startMicros = endMicros;
    detected = 0;

    if (!firstRun) {
      Serial.print("Period: ");
      Serial.println(period);
    }
  }
}

void pulseDetected() {
  if (detected) {
    error = 1;
    return;
  }
  endMicros = micros();
  detected = 1;
}

