// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"

int ledPin = 5;
long startMillis = 0;
long endMillis = 0;
char detected = 0;

void setup() {
  // setup comparator
  analogComparator.setOn(AIN0, AIN1); // sensor: D6, reference: D7 on Arduino Uno
  analogComparator.enableInterrupt(pulseDetected, RISING); // interrupt on rising edge

  pinMode(ledPin,OUTPUT);

  // init serial port
  Serial.begin(9600);
  Serial.println("* Latency measurement tool *");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  startMillis = millis();
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(1000);

  if(detected) {
    long latency = endMillis - startMillis;

    //print delay on console
    Serial.print("Detected: ");
    Serial.println(latency);

    //reset flag
    detected = 0;
  }
}

void pulseDetected() {
  endMillis = millis();
  detected = 1;
}


