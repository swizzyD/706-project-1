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
  mm = (duration / 2.0) / 2.91;

  // Rejecting out of range values and jitter
  // Recursively calling get_ultrasonic_range() until mm is a valid value
  if ((mm > 4000) || (abs(prev_mm - mm) >  ULTRASONIC_MOVE_THRESH)) {
    mm = get_ultrasonic_range();
  }

  prev_mm = mm;
  
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

//-------------------------------------------------GYRO DUMP-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro()
{
  if (Serial.available())           // Check for input from terminal
  { 
    serialRead = Serial.read();     // Read input
    if (serialRead == 49)           // Check for flag to execute, 49 is asci for 1
    { 
      Serial.end();                 // end the serial communication to display the sensor data on monitor
    }
  }
  // convert the 0-1023 signal to 0-5v
  gyroRate = (analogRead(sensorPin) * gyroSupplyVoltage) / 1023; 
  // find the voltage offset the value of voltage when gyro is zero (still)
  gyroRate -= (gyroZeroVoltage / 1023 * 5); 
  // read out voltage divided the gyro sensitivity to calculate the angular velocity
  float angularVelocity = gyroRate / gyroSensitivity; 
  // if the angular velocity is less than the threshold, ignore it
  if ((angularVelocity >= rotationThreshold) || (angularVelocity <= -rotationThreshold)) { // we are running a loop in T. one second will run (1000/T).
    float angleChange = angularVelocity / (1000 / T);
    currentAngle -= angleChange;
  }  // keep the angle between 0-360
  if (currentAngle < 0)    {
    currentAngle += 360;
  }  else if (currentAngle > 359) {
    currentAngle -= 360;
  } 
  Serial.print("Angular Velocity: ");
  Serial.print(angularVelocity);
  Serial.print(" Current Angle: ");
  Serial.println(currentAngle); 
  // control the time per loop
  //delay (T);
}
