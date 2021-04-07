#if OPEN_LOOP

bool align()
{
  return true;
}

bool forward()
{

  left_font_motor.writeMicroseconds(1500 - 400);
  left_rear_motor.writeMicroseconds(1500 - 400);
  right_rear_motor.writeMicroseconds(1500 + 400);
  right_font_motor.writeMicroseconds(1500 + 400);

  if (millis() - forward_time > 6000) {
    return true;  // movement complete
  }
  else {
    return false;  //movement imcomplete
  }
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

  if (turn_time - millis() > 1000) {
    return true;
  }
  else {
    return false;
  }
}
#endif
