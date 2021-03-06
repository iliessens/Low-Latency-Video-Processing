// library for analog comparator
// Download from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ and place in libraries folder
#include "analogComp.h"

const int ledPin = 5;
const int refPin = 9;    // reference output
const int lightADC = A0;
#define INITIAL_OFFSET 40

long startMicros = 0;
volatile long endMicros = 0;
volatile char detected = 0;

#define MICROS_PER_MILLI 1000
#define MICROSPERSECOND 1000000
long timeOut = MICROSPERSECOND * 5;

void setup() {
  // init serial port
  Serial.begin(9600);
  Serial.println("* Latency measurement tool *");

  initRef();
  pinMode(ledPin, OUTPUT);
  setOffset(INITIAL_OFFSET);

  // setup comparator
  analogComparator.setOn(AIN0, AIN1); // sensor: D6, reference: D7 on Arduino Uno

  analogComparator.enableInterrupt(pulseDetected, RISING); // interrupt on rising edge
}

void loop() {
  if (Serial.available() > 0) {
    processUI();
  }

  updateRef();
}

void measure() {
  Serial.println("Starting measurement...");
  //set flags
  detected = 0;

  //Time critical block
  digitalWrite(ledPin, HIGH);
  startMicros = micros();
  
  delay(100);
  digitalWrite(ledPin, LOW);

  //wait for detection
  while (!detected) {
    long time = micros() - startMicros;
    if (time > timeOut) break;
    delay(500);
  }

  if (detected) {
    long latency = endMicros - startMicros;

    //print delay on console
    Serial.print("Detected: ");
    Serial.println(latency);

    //reset flag
    detected = 0;

    delay(30); // make sure light pulse has died out > 1 frame delay
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
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void setRefValue() {
  int value = Serial.parseInt();
  if (value != 0) {
    Serial.print("New reference offset set to: ");
    Serial.println(value);
    setOffset(value);
  }
  else {
    Serial.println("Unknown command");
  }
}

