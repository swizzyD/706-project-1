//The Vex Motor Controller 29 use Servo Control signals to determine speed and direction, with 0 degrees meaning neutral https://en.wikipedia.org/wiki/Servo_control

static double Tr = 0; //ramp time ms



int get_ir_1()
{
  // Returns distances in cm from short IR sensor GP2Y0A41SK0F
  // 5V
  // Adapted from https://www.smart-prototyping.com/blog/Sharp-Distance-Measuring-Sensor-GP2Y0A41SK0F-Tutorial
  int distance;

  float volts = SIDE_1_READING * 0.0048828125; // value from sensor * (5/1024)
  
  distance = 13*pow(volts,-1); //side 1 distance in cm in d[0]


  return distance;
}

int get_ir_2()
{
  // Returns distances in cm from short IR sensor GP2Y0A41SK0F
  // 5V
  // Adapted from https://www.smart-prototyping.com/blog/Sharp-Distance-Measuring-Sensor-GP2Y0A41SK0F-Tutorial
  int distance;

  float volts = SIDE_2_READING * 0.0048828125; // value from sensor * (5/1024)
  
  distance = 13*pow(volts,-1); //side 1 distance in cm in d[0]


  return distance;
}

void disable_motors()
{
  left_font_motor.detach();  // detach the servo on pin left_front to turn Vex Motor Controller 29 Off
  left_rear_motor.detach();  // detach the servo on pin left_rear to turn Vex Motor Controller 29 Off
  right_rear_motor.detach();  // detach the servo on pin right_rear to turn Vex Motor Controller 29 Off
  right_font_motor.detach();  // detach the servo on pin right_front to turn Vex Motor Controller 29 Off

  pinMode(left_front, INPUT);
  pinMode(left_rear, INPUT);
  pinMode(right_rear, INPUT);
  pinMode(right_front, INPUT);
}

void enable_motors()
{
  left_font_motor.attach(left_front);  // attaches the servo on pin left_front to turn Vex Motor Controller 29 On
  left_rear_motor.attach(left_rear);  // attaches the servo on pin left_rear to turn Vex Motor Controller 29 On
  right_rear_motor.attach(right_rear);  // attaches the servo on pin right_rear to turn Vex Motor Controller 29 On
  right_font_motor.attach(right_front);  // attaches the servo on pin right_front to turn Vex Motor Controller 29 On
}

void stop() 
{
  left_font_motor.writeMicroseconds(1500);
  left_rear_motor.writeMicroseconds(1500);
  right_rear_motor.writeMicroseconds(1500);
  right_font_motor.writeMicroseconds(1500);
}

bool align()
{

  //moves side closest to wall outwards to prevent collision
  int sideMeasurement;
  if(SIDE_1_READING > SIDE_2_READING){
    sideMeasurement = SIDE_1_READING;
  }
  else{
    sideMeasurement = SIDE_2_READING;
  }

  int side_distance_correction = side_distance_PID.PID_update(sideTarget, sideMeasurement); // target, measuremet);
  int side_orientation_correction = side_orientation_PID.PID_update(0, SIDE_1_READING - SIDE_2_READING);

#if DISP_READINGS
  SerialCom->print("SIDE_1_READING - SIDE_2_READING = ");
  SerialCom->println(SIDE_1_READING - SIDE_2_READING);
  SerialCom->print("side_orientation_correction = ");
  SerialCom->println(side_orientation_correction);
  SerialCom->print("side_distance_correction = ");
  SerialCom->println(side_distance_correction);
#endif

  left_font_motor.writeMicroseconds(1500 - side_orientation_correction - side_distance_correction);
  left_rear_motor.writeMicroseconds(1500 - side_orientation_correction + side_distance_correction);
  right_rear_motor.writeMicroseconds(1500 - side_orientation_correction + side_distance_correction);
  right_font_motor.writeMicroseconds(1500 - side_orientation_correction - side_distance_correction);

  if (abs(SIDE_1_READING - SIDE_2_READING) - 10 < 10 && abs(sideTarget - SIDE_1_READING) < 5 ) {
    return true;  // movement complete
  }
  else {
    return false;  //movement imcomplete
  }

}

bool forward()
{
  
  //moves side closest to wall outwards to prevent collision
  int sideMeasurement;
  if(SIDE_1_READING > SIDE_2_READING){
    sideMeasurement = SIDE_1_READING;
  }
  else{
    sideMeasurement = SIDE_2_READING;
  }
  
  int side_distance_correction = side_distance_PID.PID_update(sideTarget, sideMeasurement); // target, measuremet);
  int side_orientation_correction = side_orientation_PID.PID_update(0, SIDE_1_READING - SIDE_2_READING); //difference of 15 to get robot straight, can change this
  int speed_val = ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());


  
#if DISP_READINGS
  SerialCom->print("SIDE_1_READING - SIDE_2_READING = ");
  SerialCom->println(SIDE_1_READING - SIDE_2_READING);
  SerialCom->print("side_orientation_correction = ");
  SerialCom->println(side_orientation_correction);
  SerialCom->print("side_distance_correction = ");
  SerialCom->println(side_distance_correction);
  SerialCom->print("ultrasonic reading = ");
  SerialCom->println(get_ultrasonic_range());
  
#endif

  left_font_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction - side_distance_correction);
  left_rear_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction + side_distance_correction);
  right_rear_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction + side_distance_correction);
  right_font_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction - side_distance_correction);

  if (abs(SIDE_1_READING - SIDE_2_READING) < 10 && abs(sideTarget - SIDE_1_READING) < 5 && abs(ultrasonicTarget - get_ultrasonic_range()) < 10) {
    return true;  // movement complete
  }
  else {
    return false;  //movement imcomplete
  }
}

void reverse ()
{
 //moves side closest to wall outwards to prevent collision
  int sideMeasurement;
  if(SIDE_1_READING > SIDE_2_READING){
    sideMeasurement = SIDE_1_READING;
  }
  else{
    sideMeasurement = SIDE_2_READING;
  }
  
  int side_distance_correction = side_distance_PID.PID_update(sideTarget, sideMeasurement); // target, measuremet);
  int side_orientation_correction = side_orientation_PID.PID_update(0, SIDE_1_READING - SIDE_2_READING); //difference of 15 to get robot straight, can change this
  int speed_val = ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());


  
#if DISP_READINGS
  SerialCom->print("SIDE_1_READING - SIDE_2_READING = ");
  SerialCom->println(SIDE_1_READING - SIDE_2_READING);
  SerialCom->print("side_orientation_correction = ");
  SerialCom->println(side_orientation_correction);
  SerialCom->print("side_distance_correction = ");
  SerialCom->println(side_distance_correction);
  SerialCom->print("ultrasonic reading = ");
  SerialCom->println(get_ultrasonic_range());
#endif

  left_font_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction - side_distance_correction);
  left_rear_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction + side_distance_correction);
  right_rear_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction + side_distance_correction);
  right_font_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction - side_distance_correction);

}

bool ccw ()
{
  //empty
}

bool cw ()
{
  
  int angular_displacement = integrate(currentAngle);
  int gyro_corr = gyro_PID.PID_update(GYRO_TARGET_ANGLE, angular_displacement); // target, measuremet);
  //int side_orientation_correction = side_orientation_PID.PID_update(0, SIDE_1_READING - SIDE_2_READING); //difference of 15 to get robot straight, can change this
  //int speed_val = ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());
  left_font_motor.writeMicroseconds(1500 - gyro_corr);
  left_rear_motor.writeMicroseconds(1500 - gyro_corr);
  right_rear_motor.writeMicroseconds(1500 - gyro_corr);
  right_font_motor.writeMicroseconds(1500 - gyro_corr);

/// Uncomment this if the robot is turning the other way OR change GYRO_TARGET_ANGLE
//  left_font_motor.writeMicroseconds(1500 + gyro_corr);
//  left_rear_motor.writeMicroseconds(1500 + gyro_corr);
//  right_rear_motor.writeMicroseconds(1500 + gyro_corr);
//  right_font_motor.writeMicroseconds(1500 + gyro_corr);

  SerialCom->print("gyro: ");
  SerialCom->println(currentAngle);

  if (abs(currentAngle - GYRO_TARGET_ANGLE) < 10) {
    integrator = 0;
    return true;
  }
  else {
    return false;
  }
    
}

void strafe_left ()
{
  int speed_val = 100;  
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void strafe_right ()
{
  int speed_val = 100;  
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}

int integrate(int val) {
  //static long integrator = 0;
  integrator += val;
  return integrator;
}
