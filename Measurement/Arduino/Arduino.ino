// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"

int ledPin = 5;

void setup() {
  // setup comparator
  analogComparator.setOn(AIN0, AIN1); 
  analogComparator.enableInterrupt(pulseDetected, RISING); // interrupt on rising edge

  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
  Serial.print("Latency measurement tool");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, LOW);
  delay(1000);
  digitalWrite(ledPin, HIGH);
  delay(1000);
}

void pulseDetected() {
  Serial.println("Detected");
}

