#if OPEN_LOOP

bool align()
{
  return true;
}

bool forward()
{
  left_font_motor.writeMicroseconds(1500 + 400);
  left_rear_motor.writeMicroseconds(1500 + 400);
  right_rear_motor.writeMicroseconds(1500 - 400);
  right_font_motor.writeMicroseconds(1500 - 400);
  delay(5200);
  return true;
}


bool forward_short()
{
  left_font_motor.writeMicroseconds(1500 + 400);
  left_rear_motor.writeMicroseconds(1500 + 400);
  right_rear_motor.writeMicroseconds(1500 - 400);
  right_font_motor.writeMicroseconds(1500 - 400);

  delay(2800);
  return true;
}

void reverse ()
{
}

bool ccw ()
{
  //empty
  return true;
}

bool cw ()
{


  left_font_motor.writeMicroseconds(1500 + 200);
  left_rear_motor.writeMicroseconds(1500 + 200);
  right_rear_motor.writeMicroseconds(1500 + 200);
  right_font_motor.writeMicroseconds(1500 + 200);

  delay(1100);
  return true;
}
#endif
