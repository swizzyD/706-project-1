
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
  int side_distance_correction = side_distance_PID.PID_update(sideTarget, SIDE_1_READING); // target, measuremet);
  int side_orientation_correction = side_orientation_PID.PID_update(15, SIDE_1_READING - SIDE_2_READING); //difference of 15 to get robot straight, can change this
  int speed_val = Ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());

#if DISP_READINGS
  SerialCom->print("side difference = ");
  SerialCom->println(SIDE_1_READING - SIDE_2_READING);
  SerialCom->print("side controller output = ");
  SerialCom->println(side_orientation_correction);
  SerialCom->print("front reading = ");
  SerialCom->println(get_ultrasonic_range());
#endif

  left_font_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction - side_distance_correction);
  left_rear_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction + side_distance_correction);
  right_rear_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction + side_distance_correction);
  right_font_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction - side_distance_correction);

  if (abs(SIDE_1_READING - SIDE_2_READING) < 20 && abs(sideTarget - SIDE_1_READING) < 5 && abs(ultrasonicTarget - get_ultrasonic_range()) < 10) {
    return true;  // movement complete
  }
  else {
    return false;  //movement imcomplete
  }
}

void reverse ()
{
  int side_distance_correction = side_distance_PID.PID_update(335, SIDE_1_READING); // target, measuremet);
  int side_orientation_correction = side_orientation_PID.PID_update(15, SIDE_1_READING - SIDE_2_READING);
  int speed_val = Ultrasonic_PID.PID_update(ultrasonicTarget, get_ultrasonic_range());

#if DISP_READINGS
  SerialCom->print("side difference = ");
  SerialCom->println(SIDE_1_READING - SIDE_2_READING);
  SerialCom->print("side controller output = ");
  SerialCom->println(side_orientation_correction);
  SerialCom->print("front reading = ");
  SerialCom->println(get_ultrasonic_range());
#endif

  left_font_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction - side_distance_correction);
  left_rear_motor.writeMicroseconds(1500 + speed_val - side_orientation_correction + side_distance_correction);
  right_rear_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction + side_distance_correction);
  right_font_motor.writeMicroseconds(1500 - speed_val - side_orientation_correction - side_distance_correction);

}

void ccw ()
{
  int speed_val = 100;  //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

int cw ()
{
  int angular_displacement = integrator(GYRO_READING - 499);
  int turning_val = gyro_PID.PID_update(angular_displacement, turnTarget);
  left_font_motor.writeMicroseconds(1500 - turning_val);
  left_rear_motor.writeMicroseconds(1500 - turning_val);
  right_rear_motor.writeMicroseconds(1500 - turning_val);
  right_font_motor.writeMicroseconds(1500 - turning_val);

  if (abs(angular_displacement - turnTarget) < 100) {
    return true;
  }
  else {
    return false;
  }
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

int integrator(int val) {
  static long integrator = 0;
  integrator += val;
  return integrator;
}
