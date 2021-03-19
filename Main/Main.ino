/*
  Hardware:
    Arduino Mega2560 https://www.arduino.cc/en/Guide/ArduinoMega2560
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

#include <math.h>

//#define NO_BATTERY_V_OK //Uncomment of BATTERY_V_OK if you do not care about battery damage.
#define DISP_READINGS 1
#define SAMPLING_TIME 20 //ms , operate at 50Hz
#define GYRO_READING analogRead(A3)
#define SIDE_1_READING analogRead(A4)
#define SIDE_2_READING analogRead(A6)


//---------maybe remove these if alpha angle isn't going to be used----------
//---------------------------------------------------------------------------
#define DIST_BETWEEN_IR 12 //cm NEED TO MEASURE
#define SIDE_DIST_TARGET 15 - 7.5 //distance_to_ir_from_centre in cm CALCULATE THIS AND REPLACE 0
//-----------------------------------------------------------------------------


#define GYRO_TARGET_ANGLE 90
#define ULTRASONIC_MOVE_THRESH 25



//machine states
enum STATE {
  INITIALISING,
  RUNNING,
  STOPPED
};

//-------------------------------------------GYRO VARIABLES--------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sensorPin = 3;             //define the pin that gyro is connected
int T = 100;                    // T is the time of one loop
int sensorValue = 0;            // read out value of sensor
float gyroSupplyVoltage = 5;    // supply voltage for gyro
float gyroZeroVoltage = 0;      // the value of voltage when gyro is zero
float gyroSensitivity = 0.007;  // gyro sensitivity unit is (v/degree/second) get from datasheet
float rotationThreshold = 1.5;  // because of gyro drifting, defining rotation angular velocity  less than                                                        // this value will not be ignored
float gyroRate = 0;             // read out value of sensor in voltage
float currentAngle = 0;         // current angle calculated by angular velocity integral on
byte serialRead = 0;            // for serial print control



//-------------------------------PID OBJECTS-----// Kp, Ki, Kd, limMin, limMax

PID gyro_PID(3.0f, 0.01f, 0.0f, -200, 200);
PID side_distance_PID(1.0f, 0.01f, 0.0f, -200, 200);
PID side_orientation_PID(2.0f, 0.005f, 0.0f, -200, 200);
PID Ultrasonic_PID(0.03f, 0.001f, 0.0f, -200, 200);

PID alpha_correction(1.0f, 0.0f, 0.0f, -200, 200);
PID side_dist_corr(1.0f, 0.0f, 0.0f, -200, 200);


static int turnTarget = 20000;
static int sideTarget = 280;
//static int ultrasonicTarget = 580; // pulse width not cm
static int ultrasonicTarget = 90; //150 - (235/2.0) - 15;//in mm (235/2) is half of robot length, 15 is length of ultrasonic sensor NEEDS TO CHANGE AFTER ULTRASONIC SENSOR MOUNTING

// Variable for get_ultrasonic_range()
static int prev_mm = 0;




//-----------------Default motor control pins--------------
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;
//---------------------------------------------------------------------------------------------------------


//-----------Ultrasonic pins--------------------------------------------------------
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
  SerialCom->println("Setup....");
  SerialCom->println("PID init....");


  //-----------------------------------GYRO SETUP----------------------------------------------------------------
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // this section is initialize the sensor, find the the value of voltage when gyro is zero
  int i;
  float sum = 0;
  pinMode(sensorPin, INPUT);
  Serial.println("please keep the sensor still for calibration");
  Serial.println("get the gyro zero voltage");
  for (i = 0; i < 100; i++)     //  read 100 values of voltage when gyro is at still, to calculate the zero-drift
  {
    sensorValue = analogRead(sensorPin);
    sum += sensorValue;
  }
  delay(5);

  gyroZeroVoltage = sum / 100;  // average the sum as the zero drifting
  Serial.print("Zero Voltage: ");
  Serial.println(gyroZeroVoltage);





  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

  delay(1000); //settling time but no really needed

}

void loop(void)
{
  static STATE machine_state = INITIALISING;

  switch (machine_state) {
    case INITIALISING:
      machine_state = initialising();
      break;
    case RUNNING: //includes Lipo Battery check
      machine_state =  running();
      break;
    case STOPPED: //Stop of Lipo Battery voltage is too low, to protect Battery
      machine_state =  stopped();
      break;
  };
}


//--------------- MACHINE STATES------------------------------

STATE initialising() {
  //initialising
  SerialCom->println("INITIALISING....");
  SerialCom->println("Enabling Motors...");
  enable_motors();
  SerialCom->println("ADJUSTMENT STATE...");
  return RUNNING;
}


STATE running() {

  static unsigned long previous_millis_1;
  static unsigned long previous_millis_2;
  static int movement_state = 1;
  static bool movement_complete = false;
  static int count = 0;

  fast_flash_double_LED_builtin();

  //-----------------MOVEMENT STATE MACHINE---------------------
  if (millis() - previous_millis_1 > SAMPLING_TIME) {
    SerialCom ->print("movement state = ");
    SerialCom->println(movement_state);
    previous_millis_1 = millis();

    if (movement_state == 0) {
      stop();
      return STOPPED;
    }

    else if (movement_state == 1) {
      movement_complete = align();
      if (movement_complete) {
        movement_state = 2;
      }
      else if (!movement_complete) {
        movement_state = 1;
      }
    }

    else if (movement_state == 2) {
      movement_complete = forward();
      if (movement_complete) {
        movement_state = 3;
      }
      else if (!movement_complete) {
        movement_state = 2;
      }
    }

    else if (movement_state == 3) {
      movement_complete = cw();
      if (movement_complete && count != 3) {
        movement_state = 2; // Change to movement_state = 1 so that the robot aligns after the turn? final pos due to gyro may not be reliable
//        movement_state = 1;
        count++;
      }
      else if (movement_complete && count == 3) {
        movement_state = 0;
      }
      else if (!movement_complete && count != 3) {
        movement_state = 3;
      }
    }
    
  }


  if (millis() - previous_millis_2 > 500) {  
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
    ultrasonic_reading();


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
