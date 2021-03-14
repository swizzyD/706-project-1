/*
  Hardware:
    Arduino Mega2560 https://www.arduino.cc/en/Guide/ArduinoMega2560
    MPU-9250 https://www.sparkfun.com/products/13762
    Ultrasonic Sensor - HC-SR04 https://www.sparkfun.com/products/13959
    Infrared Proximity Sensor - Sharp https://www.sparkfun.com/products/242
    Infrared Proximity Sensor Short Range - Sharp https://www.sparkfun.com/products/12728
    Servo - Generic (Sub-Micro Size) https://www.sparkfun.com/products/9065
    Vex Motor Controller 29 https://www.vexrobotics.com/276-2193.html
    Vex Motors https://www.vexrobotics.com/motors.html
    Turnigy nano-tech 2200mah 2S https://hobbyking.com/en_us/turnigy-nano-tech-2200mah-2s-25-50c-lipo-pack.html
*/

#include <Servo.h>  //Need for Servo pulse output
#include "PID_class.h"

//#define NO_BATTERY_V_OK //Uncomment of BATTERY_V_OK if you do not care about battery damage.
#define DISP_READINGS 1
#define SAMPLING_TIME 20 //ms , operate at 50Hz
#define GYRO_READING analogRead(A3)
#define SIDE_1_READING analogRead(A4)
#define SIDE_2_READING analogRead(A6)
#define FRONT_READING analogRead(A5)

//-------------------------------PID OBJECTS---------------------------------
PID gyro_PID(3.0f, 0.01f, 0.0f, -200, 200);  // Kp, Ki, Kd, limMin, limMax
PID side_distance_PID(5.0f, 0.005f, 0.0f, -200, 200);
PID side_orientation_PID(2.0f, 0.005f, 0.0f, -200, 200);
PID Ultrasonic_PID(1.0f, 0.0f, 0.0f, -200, 200);   // Kp, Ki, Kd, limMin, limMax

static int gyroTarget = 0;
static int sideTarget = 335;
static int frontTarget = 580; // pulse width not cm

//------------------------------------------------------------------------------

//State machine states
enum STATE {
  INITIALISING,
  RUNNING,
  STOPPED
};

//-----------------Default motor control pins--------------
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;
//---------------------------------------------------------------------------------------------------------


//-----------Default ultrasonic ranging sensor pins, these pins are defined my the Shield-------------------
const int TRIG_PIN = 48;
const int ECHO_PIN = 49;

// Anything over 400 cm (23200 us pulse) is "out of range".
// Hint:If you decrease to this the ranging sensor but the timeout is short, you may not need to read up to 4meters.
const unsigned int MAX_DIST = 23200;
//--------------------------------------------------------------------------------------------------------------

//----------------------Servo Objects---------------------------------------------------------------------------
Servo left_font_motor;  // create servo object to control Vex Motor Controller 29
Servo left_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_font_motor;  // create servo object to control Vex Motor Controller 29
Servo turret_motor;
//-----------------------------------------------------------------------------------------------------------


//Serial Pointer
HardwareSerial *SerialCom;

int pos = 0;
void setup(void)
{
  turret_motor.attach(11);
  pinMode(LED_BUILTIN, OUTPUT);

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial;
  SerialCom->begin(115200);
  SerialCom->println("MECHENG706_Base_Code_25/01/2018");
  SerialCom->println("Setup....");
  SerialCom->println("PID init....");


  delay(1000); //settling time but no really needed

}

void loop(void)
{
  static STATE machine_state = INITIALISING;
  //Finite-state machine Code
  switch (machine_state) {
    case INITIALISING:
      machine_state = initialising();
      break;
    case RUNNING: //Lipo Battery Volage OK
      machine_state =  running();
      break;
    case STOPPED: //Stop of Lipo Battery voltage is too low, to protect Battery
      machine_state =  stopped();
      break;
  };
}


//---------------STATES------------------------------



STATE initialising() {
  //initialising
  SerialCom->println("INITIALISING....");
  delay(1000); //One second delay to see the serial string "INITIALISING...."
  SerialCom->println("Enabling Motors...");
  enable_motors();
  SerialCom->println("RUNNING STATE...");
  update_sensor_targets();    //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  return RUNNING;
}

STATE running() {

  static unsigned long previous_millis_1;
  static unsigned long previous_millis_2;
  static int movement_state = 1;
  static bool movement_complete = false;

  fast_flash_double_LED_builtin();

//-----------------MOVEMENT STATE MACHINE---------------------
  if (millis() - previous_millis_1 > SAMPLING_TIME) {    
    previous_millis_1 = millis();
    if (movement_state == 0) {
      stop();
      return STOPPED;
    }
    else if (movement_state == 1) {
      movement_complete = forward();
      if (movement_complete) {
        movement_state = 0;
      }
      else {
        movement_state = 1;
      }
    }
    else if (movement_state == 2){
      movement_complete = cw();
      if(movement_complete){
        movement_state = 1;
      }
      else{
        movement_state = 2;
      }
    }
    
  }
//---------------------------------------------------------------

  if (millis() - previous_millis_2 > 500) {  //Arduino style 500ms timed execution statement
    previous_millis_2 = millis();
    SerialCom->println("RUNNING---------");

#ifndef NO_BATTERY_V_OK
    if (!is_battery_voltage_OK()) return STOPPED;
#endif


    turret_motor.write(pos);
    if (pos == 0)
    {
      pos = 45;
    }
    else
    {
      pos = 0;
    }
  }

  return RUNNING;
}


//Stop of Lipo Battery voltage is too low, to protect Battery
STATE stopped() {
  static byte counter_lipo_voltage_ok;
  static unsigned long previous_millis;
  int Lipo_level_cal;
  disable_motors();
  slow_flash_LED_builtin();

  if (millis() - previous_millis > 500) { //print massage every 500ms
    previous_millis = millis();
    SerialCom->println("STOPPED---------");
    
    gyro_reading();
    side_reading();
    front_reading();
    UltraSonic_range();


#ifndef NO_BATTERY_V_OK
    //500ms timed if statement to check lipo and output speed settings
    if (is_battery_voltage_OK()) {
      SerialCom->print("Lipo OK waiting of voltage Counter 10 < ");
      SerialCom->println(counter_lipo_voltage_ok);
      counter_lipo_voltage_ok++;
      if (counter_lipo_voltage_ok > 10) { //Making sure lipo voltage is stable
        counter_lipo_voltage_ok = 0;
        enable_motors();
        SerialCom->println("Lipo OK returning to RUN STATE");
        return RUNNING;
      }
    } else
    {
      counter_lipo_voltage_ok = 0;
    }
#endif
  }
  return STOPPED;
}
