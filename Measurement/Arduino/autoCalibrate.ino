#define CALIBRATESTEPS 16

void calibrate() {
  Serial.println("Starting auto calibration...");
  // enable led
  digitalWrite(ledPin, HIGH);

  int i =0;
  int success = 1;
  int value = 0;
  
  while(success && (i <= CALIBRATESTEPS))  {
    
    value = i * 255/CALIBRATESTEPS;
    Serial.print("Trying value: ");
    Serial.println(value);
    
    analogWrite(refPin, value);
    delay(2000); // wait for output to stabilize
    
    // check comparator
    success = analogComparator.waitComp(100);
    Serial.println(success);

    ++i;
  }

  if(!success) {
    Serial.print("Calibrated with value: ");
    Serial.println(value);
  }
  else Serial.println("Calibration failed");
  
}

