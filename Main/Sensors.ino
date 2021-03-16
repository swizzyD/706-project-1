

int get_ultrasonic_range()
{
//  unsigned long t1;
//  unsigned long t2;
//  unsigned long pulse_width;
//  float cm;

//  // Hold the trigger pin high for at least 10 us
//  digitalWrite(TRIG_PIN, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(TRIG_PIN, LOW);
//
//  // Wait for pulse on echo pin
//  t1 = micros();
//  while ( digitalRead(ECHO_PIN) == 0 ) {
//    t2 = micros();
//    pulse_width = t2 - t1;
//    if ( pulse_width > (MAX_DIST + 1000)) {
//      return 9999;
//    }
//  }
//
//  // Measure how long the echo pin was held high (pulse width)
//  // Note: the micros() counter will overflow after ~70 min
//
//  t1 = micros();
//  while ( digitalRead(ECHO_PIN) == 1)
//  {
//    t2 = micros();
//    pulse_width = t2 - t1;
//    if ( pulse_width > (MAX_DIST + 1000) ) {
//      SerialCom->println("Ultrasonic: Out of range");
//      return 9999;
//    }
//  }
//
//  t2 = micros();
//  pulse_width = t2 - t1;
//  return pulse_width;
/*
  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    SerialCom->println("Ultrasonic: Out of range");
  } else {
    SerialCom->print("Ultrasonic:");
    SerialCom->print(cm);
    SerialCom->println("cm");
  }
  */

  //-------------SAM CODE------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
  // adapted from Rui Santos, https://randomnerdtutorials.com
  long duration, mm;
  
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  // Clearing the trig pin
  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(5);
  // pulsing high for at least 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);


  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);

  // Convert the time into a distance
  mm = ((duration*10)/2) / 29.1;

  return mm;
}



void side_reading()
{
  SerialCom->print("side 1_reading:");
  SerialCom->println(SIDE_1_READING);
  SerialCom->print("side 2_reading:");
  SerialCom->println(SIDE_2_READING);
}

void ultrasonic_reading()
{
  SerialCom->print("ultrasonic reading:");
  SerialCom->println(get_ultrasonic_range());
}

void gyro_reading()
{
  SerialCom->print("gyro reading:");
  SerialCom->println(GYRO_READING);
}
