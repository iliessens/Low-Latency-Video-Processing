// Moving average filter adapted from https://www.arduino.cc/en/tutorial/smoothing

const long updatePeriod = 300; // how often is the ADC sampled
const int numReadings = 10; // how many readings do we average. Has an important impact on memory usage

int refOffset = 0; // offset of the reference value

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
long prevTime = 0;

void initRef() {
  pinMode(refPin, OUTPUT);   // sets the pin as output
  analogWrite(refPin, 128);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 128; // Start ref at half value
  }
}

void updateRef() {
  if (millis() - prevTime > updatePeriod) {
    int inputvoltage = measureLightMean();
    int outvoltage = inputvoltage / 4; // output resolution is lower
    writeRefValue(outvoltage);

    prevTime = millis();
  }
}

int measureLightMean() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(lightADC);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    // wrap around to the beginning:
    readIndex = 0;
  }

  return total / numReadings;
}

int writeRefValue(int value) {
  value = value + refOffset;
  if (value < 0) {
    analogWrite(refPin, 0);
    return 0;
  }
  else if (value > 255) {
    analogWrite(refPin, 255);
    return 0;
  }
  else {
    analogWrite(refPin, value);
    return 1;
  }
}

void setOffset(int value) {
  refOffset = value;
}

