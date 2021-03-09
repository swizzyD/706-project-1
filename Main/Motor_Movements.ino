
//The Vex Motor Controller 29 use Servo Control signals to determine speed and direction, with 0 degrees meaning neutral https://en.wikipedia.org/wiki/Servo_control

static double Tr = 0; //ramp time ms

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
void stop() //Stop
{
  left_font_motor.writeMicroseconds(1500);
  left_rear_motor.writeMicroseconds(1500);
  right_rear_motor.writeMicroseconds(1500);
  right_font_motor.writeMicroseconds(1500);
}

bool forward()
{

  int gyro_correction = gyro_PID.PID_update(gyroTarget, GYRO_READING); // target, measurement
  int side_correction = side_PID.PID_update(sideTarget, SIDE_READING); // target, measuremet
  int speed_val = front_PID.PID_update(frontTarget, FRONT_READING);    
  
#if DISP_READINGS
  SerialCom->print("gyro controller output = ");
  SerialCom->println(gyro_correction);
  SerialCom->print("gyro reading = ");
  SerialCom->println(GYRO_READING);
  SerialCom->print("side controller output = ");
  SerialCom->println(side_correction);
  SerialCom->print("side reading = ");
  SerialCom->println(SIDE_READING);
  SerialCom->print("front controller output = ");
  SerialCom->println(speed_val);
  SerialCom->print("side reading = ");
  SerialCom->println(FRONT_READING);
#endif

  int rampOut = ramp(speed_val, Tr);
  left_font_motor.writeMicroseconds(1500 + rampOut + gyro_correction + side_correction);
  left_rear_motor.writeMicroseconds(1500 + rampOut + gyro_correction - side_correction);
  right_rear_motor.writeMicroseconds(1500 - rampOut + gyro_correction - side_correction);
  right_font_motor.writeMicroseconds(1500 - rampOut + gyro_correction + side_correction);

  if(abs(gyroTarget - GYRO_READING) < 5 && abs(sideTarget - SIDE_READING) < 5 && abs(frontTarget - FRONT_READING) < 5){
    return true;  // movement complete
  }
  else{
    return false;  //movement imcomplete
  }
}

void reverse ()
{
  int gyro_correction = gyro_PID.PID_update(gyroTarget, GYRO_READING); // target, measurement
  int side_correction = side_PID.PID_update(sideTarget, SIDE_READING); // target, measurement
  int speed_val = front_PID.PID_update(frontTarget, FRONT_READING); 
  int rampOut = ramp(speed_val, Tr);

  left_font_motor.writeMicroseconds(1500 - rampOut + gyro_correction + side_correction);
  left_rear_motor.writeMicroseconds(1500 - rampOut + gyro_correction - side_correction);
  right_rear_motor.writeMicroseconds(1500 + rampOut + gyro_correction - side_correction);
  right_font_motor.writeMicroseconds(1500 + rampOut + gyro_correction + side_correction);

#if DISP_READINGS
  SerialCom->print("gyro controller output = ");
  SerialCom->println(gyro_correction);
  SerialCom->print("gyro reading = ");
  SerialCom->println(GYRO_READING);
  SerialCom->print("side controller output = ");
  SerialCom->println(side_correction);
  SerialCom->print("side reading = ");
  SerialCom->println(SIDE_READING);
  SerialCom->print("front controller output = ");
  SerialCom->println(speed_val);
  SerialCom->print("side reading = ");
  SerialCom->println(FRONT_READING);
#endif
}

void ccw ()
{
  int speed_val = 100;  //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void cw ()
{
  int speed_val = 100;  //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}

void strafe_left ()
{
  int speed_val = 100;  //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void strafe_right ()
{
  int speed_val = 100;  //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}



int ramp(int val, double t) {
  int ramp_out;
  if (millis() - t < 2000) {
    ramp_out = val * (millis() - t) / 2000;
  }
  else {
    ramp_out = val;
  }
  return ramp_out;
}
