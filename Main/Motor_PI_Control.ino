
//--------------Gyro PI_controller VARS-------------
  static float gyro_Kp = 1.0f;
  static float gyro_Ki = 0.01f;
  static int gyro_limMin = -250;
  static int gyro_limMax = 250;


  static float gyro_integrator = 0.0f;
  static float gyro_prevError = 0.0f;
  static int gyro_out = 0;
//--------------------------------------------------

  static int Ts = 20;  //sampling time ms

//--------------Side distance PI_controller VARS-------------
  static float side_Kp = 6.0f;
  static float side_Ki = 0.005f;
  static int side_limMin = -250;
  static int side_limMax = 250;
  static float side_integrator = 0.0f;
  static float side_prevError = 0.0f;
  static int side_out = 0;
//--------------------------------------------------


int gyro_PI_controller_update(int gyro){
  int error;

    error = gyroSteadyState - gyro;

  float proportional = gyro_Kp * error;
  gyro_integrator += 0.5f * gyro_Ki * Ts * (error + gyro_prevError);

  //integrator dynamic clamp
  float limMaxIntegrator, limMinIntegrator;

  if(gyro_limMax > proportional){
    limMaxIntegrator = gyro_limMax - proportional;
  }
  else{
    limMaxIntegrator = 0.0f;
  }

  if(gyro_limMin < proportional){
    limMinIntegrator = gyro_limMin - proportional;
  }
  else{
    limMinIntegrator = 0.0f;
  }

  //clamp integrator
  if(gyro_integrator > limMaxIntegrator){
    gyro_integrator = limMaxIntegrator;
  }
  if(gyro_integrator < limMinIntegrator){
    gyro_integrator = limMinIntegrator;
  }

  gyro_out = proportional + gyro_integrator;
  
  //clamp output
  if(gyro_out > gyro_limMax){
    gyro_out = gyro_limMax;
  }
  if(proportional + gyro_integrator < gyro_limMin){
    gyro_out = gyro_limMin;
  }

  gyro_prevError = error;

  
  SerialCom->print("gyro controller output = ");
  SerialCom->println(gyro_out);
  SerialCom->print("gyro output = ");
  SerialCom->println(analogRead(A3));
  return gyro_out;
  
}



int side_PI_controller_update(int side){
  int error;

    error = sideSteadyState - side;

  float proportional = side_Kp * error;
  side_integrator += 0.5f * side_Ki * Ts * (error + side_prevError);

  //integrator dynamic clamp
  float limMaxIntegrator, limMinIntegrator;

  if(side_limMax > proportional){
    limMaxIntegrator = side_limMax - proportional;
  }
  else{
    limMaxIntegrator = 0.0f;
  }

  if(side_limMin < proportional){
    limMinIntegrator = side_limMin - proportional;
  }
  else{
    limMinIntegrator = 0.0f;
  }

  //clamp integrator
  if(side_integrator > limMaxIntegrator){
    side_integrator = limMaxIntegrator;
  }
  if(side_integrator < limMinIntegrator){
    side_integrator = limMinIntegrator;
  }

  side_out = proportional + side_integrator;
  
  //clamp output
  if(side_out > side_limMax){
    side_out = side_limMax;
  }
  if(proportional + side_integrator < side_limMin){
    side_out = side_limMin;
  }

  side_prevError = error;

  
  SerialCom->print("side controller output = ");
  SerialCom->println(side_out);
  SerialCom->print("side output = ");
  SerialCom->println(analogRead(A4));
  return side_out;
  
}


int ramp(int val, double t){
  int ramp_out;
  if(millis() - t < 2000){
    ramp_out = val * (millis() - t)/2000;
  }
  else{
    ramp_out = val;
  }
  return ramp_out;
}
