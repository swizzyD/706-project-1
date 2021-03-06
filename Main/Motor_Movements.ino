
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

void forward()
{ 
  int gyro_correction = gyro_PI_controller_update(analogRead(A3));
  int side_correction = side_PI_controller_update(analogRead(A4));
  int rampOut = ramp(speed_val,Tr);
  left_font_motor.writeMicroseconds(1500 + rampOut + gyro_correction + side_correction);
  left_rear_motor.writeMicroseconds(1500 + rampOut + gyro_correction - side_correction);
  right_rear_motor.writeMicroseconds(1500 - rampOut + gyro_correction - side_correction);
  right_font_motor.writeMicroseconds(1500 - rampOut + gyro_correction + side_correction);
}

void reverse ()
{
  int gyro_correction = gyro_PI_controller_update(analogRead(A3));
  int side_correction = side_PI_controller_update(analogRead(A4));
  int rampOut = ramp(speed_val,Tr);
  left_font_motor.writeMicroseconds(1500 - rampOut + gyro_correction + side_correction);
  left_rear_motor.writeMicroseconds(1500 - rampOut + gyro_correction - side_correction);
  right_rear_motor.writeMicroseconds(1500 + rampOut + gyro_correction - side_correction);
  right_font_motor.writeMicroseconds(1500 + rampOut + gyro_correction + side_correction);
}

void ccw ()
{
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void cw ()
{
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}

void strafe_left ()
{
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void strafe_right ()
{
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}

void speed_change_smooth()
{
    speed_val += speed_change;

  if (speed_val > 1000)
    speed_val = 1000;
  speed_change = 0;
}
