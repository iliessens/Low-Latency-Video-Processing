// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"

int ledPin = 5;
long startMicros = 0;
long endMicros = 0;
char detected = 0;

#define MICROSPERSECOND 1000000
long timeOut = MICROSPERSECOND * 2;

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
  startMicros = micros();
  delay(100);
  digitalWrite(ledPin, LOW);

  //wait for detection
  while(!detected) {
    long time = micros() - startMicros;
    if(time > timeOut) break;
    delay(1000);
  }

  if(detected) {
    long latency = endMicros - startMicros;
  
    //print delay on console
    Serial.print("Detected: ");
    Serial.println(latency);
  
    //reset flag
    detected = 0;
  }
  else {
    Serial.println("Error measurement timed out. Check placement");
  }

  delay(1000); // wait 1s before next measurement
}

void pulseDetected() {
  endMicros = micros();
  detected = 1;
}


