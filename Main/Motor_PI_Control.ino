
//--------------PI_controller VARS-------------
  static float Kp = 1.0f;
  static float Ki = 0.01f;
  static int limMin = -500;
  static int limMax = 500;
  static int Ts = 20;  //sampling time ms

  static float integrator = 0.0f;
  static float prevError = 0.0f;
  static int out = 0;
//--------------------------------------------------


int PI_controller_update(int gyro){
  int error;
  if(abs(gyroSteadyState - gyro) > 1){
    error = gyroSteadyState - gyro;
  }
  else{
    error = 0;
  }
  float proportional = Kp * error;
  integrator += 0.5f * Ki * Ts * (error + prevError);

  //integrator dynamic clamp
  float limMaxIntegrator, limMinIntegrator;

  if(limMax > proportional){
    limMaxIntegrator = limMax - proportional;
  }
  else{
    limMaxIntegrator = 0.0f;
  }

  if(limMin < proportional){
    limMinIntegrator = limMin - proportional;
  }
  else{
    limMinIntegrator = 0.0f;
  }

  //clamp integrator
  if(integrator > limMaxIntegrator){
    integrator = limMaxIntegrator;
  }
  if(integrator < limMinIntegrator){
    integrator = limMinIntegrator;
  }

  out = proportional + integrator;
  
  //clamp output
  if(out > limMax){
    out = limMax;
  }
  if(proportional + integrator < limMin){
    out = limMin;
  }

  prevError = error;

  
  SerialCom->print("controller output = ");
  SerialCom->println(out);
  SerialCom->print("gyro output = ");
  SerialCom->println(analogRead(A3));
  return out;
  
}


int ramp(int val, double t){
  if(millis() - t < 2000){
    out = val * (millis() - t)/2000;
  }
  else{
    out = val;
  }
  return out;
}
