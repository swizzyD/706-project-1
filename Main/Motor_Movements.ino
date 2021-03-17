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
  int speed_val = Ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());


  
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
  int speed_val = Ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());


  
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

int ccw ()
{
//  int speed_val = 100;  //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  left_font_motor.writeMicroseconds(1500 - speed_val);
//  left_rear_motor.writeMicroseconds(1500 - speed_val);
//  right_rear_motor.writeMicroseconds(1500 - speed_val);
//  right_font_motor.writeMicroseconds(1500 - speed_val);

//-----------------------------------SAM CODE-------------------------------------------
    int distance_side_1 = get_ir_1;
    int distance_side_2 = get_ir_2;
    
    

    int alpha = atan2((distance_side_2 - distance_side_1),DIST_BETWEEN_IR); // angle alpha in radians
    int mid_dist = (DIST_BETWEEN_IR/2.0) * tan(alpha); //approximate distance from middle-left edge of robot to the wall
    alpha = alpha  * (71/4068.0); // degrees conversion
    int dist_corr = side_dist_corr.PID_update(SIDE_DIST_TARGET, mid_dist);
    int a_corr = alpha_correction.PID_update(0, alpha); 
    
    if ((distance_side_1 < 30) && (distance_side_2 < 30)) {
      // if sensors are in range, move by a controlled amount
      left_font_motor.writeMicroseconds(1500 - a_corr - dist_corr);
      left_rear_motor.writeMicroseconds(1500 - a_corr + dist_corr);
      right_rear_motor.writeMicroseconds(1500 - a_corr + dist_corr);
      right_font_motor.writeMicroseconds(1500 - a_corr - dist_corr);
    }

    // OUT OF RANGE CASE??

    if (abs(distance_side_2 - distance_side_1) < 2) {
      // difference between sensor reading 2cm threshold
      return true;
    }
    else {
      return false;
    }
}

int cw ()
{
//  int angular_displacement = integrator(GYRO_READING - 499);
//  int turning_val = gyro_PID.PID_update(angular_displacement, turnTarget);
//  left_font_motor.writeMicroseconds(1500 - turning_val);
//  left_rear_motor.writeMicroseconds(1500 - turning_val);
//  right_rear_motor.writeMicroseconds(1500 - turning_val);
//  right_font_motor.writeMicroseconds(1500 - turning_val);
//
//
// 
//
//  if (abs(angular_displacement - turnTarget) < 100) {
//    return true;
//  }
//  else {
//    return false;
//  }
    


//-----------------------------------SAM CODE------------------------------------------
// This function rotates the robot clockwise and is used in turning
    int distance_side_1 = get_ir_1;
    int distance_side_2 = get_ir_2;
    
    

    int alpha = atan2((distance_side_2 - distance_side_1),DIST_BETWEEN_IR); // angle alpha in radians
    int mid_dist = (DIST_BETWEEN_IR/2.0) * tan(alpha); //approximate distance from middle-left edge of robot to the wall
    alpha = alpha  * (71/4068.0); // degrees conversion
    int dist_corr = side_dist_corr.PID_update(SIDE_DIST_TARGET, mid_dist);
    int a_corr = alpha_correction.PID_update(0, alpha); 

    while (get_ultrasonic_range < 100) {
      // While ultrasonic doesn't read the far side wall, turn clockwise
      // 100 is a semi-arbitrary value, just need the robot to rotate far enough that the below if-statement doesn't get triggered at the close-left wall
      left_font_motor.writeMicroseconds(1500 + 200);
      left_rear_motor.writeMicroseconds(1500 + 200);
      right_rear_motor.writeMicroseconds(1500 + 200);
      right_font_motor.writeMicroseconds(1500 + 200);
    }
    // POTENTIAL PROBLEM WITH ABOVE CODE AND BELOW CODE IF THE TOP CODE BLOCK CAUSES OVER-ROTATION
 
    if ((distance_side_1 < 30) && (distance_side_2 < 30)) {
      // if sensors are in range, move by a controlled amount
      left_font_motor.writeMicroseconds(1500 + a_corr - dist_corr);
      left_rear_motor.writeMicroseconds(1500 + a_corr + dist_corr);
      right_rear_motor.writeMicroseconds(1500 + a_corr + dist_corr);
      right_font_motor.writeMicroseconds(1500 + a_corr - dist_corr);
    }
    // DOES THIS IF-STATEMENT NEED AN OUT-OF-RANGE CASE?


    if (abs(distance_side_2 - distance_side_1) < 2) {
      // difference between sensor reading 2cm threshold
      return true;
    }
    else {
      return false;
    }
    
    
    
}

void align2 ()
{
// NEEDS FIXING IF TO BE USED----------------------------------------------------------------------------------------------------------------------------------------------
// NEEDS FIXING IF TO BE USED----------------------------------------------------------------------------------------------------------------------------------------------
// NEEDS FIXING IF TO BE USED----------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------SAM CODE---------------------------------
  // tbh this looks more like a align function rather than a 90 deg ccw rotate
    int distance_to_ir_from_centre = 0;
    int distance_target = 150 - distance_to_ir_from_centre;
    int distance_side_1 = get_ir_1;
    int distance_side_2 = get_ir_2;



    int alpha = atan2((distance_side_2 - distance_side_1),DIST_BETWEEN_IR) * (71/4068); // angle alpha in degrees

    if (abs(alpha) < 2) {
      // within 2 degree threshold
      return 1;
    }
    else {
      // if angle alpha is greater than zero, robot needs to rotate right
      if (alpha > 0) {
        
      }
      // if angle alpha is less than zero, robot needs to rotate left
      else if (alpha < 0) {
        
      }
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

int integrator(int val) {
  static long integrator = 0;
  integrator += val;
  return integrator;
}
