# Walter

## Logs

### 3 Mar
  * Reorganize code into sections
  * Connected + display gyroscope readings
  * Added PI control using gyro to drive straight 

### 5 Mar
  * Tune gyro PI
  * ramp function to eliminate startup jitter
    * note - gyro only ensures constant orientation, robot still strafes sideways. Use distance sensor to eliminate strafe.

### 6 Mar
  * Added side distance sensor PI
  * Reform failed - try again later

## 9 Mar
  * PID class in use
  * ditched serial command for movements. instead using state machine to update movements
  * Added code for front sensor, sensor not connected

## 15 Mar
  * ultrasonic code implemented
  * turning function to be improved
  * all controllers need tuning

## 16 Mar
  * Implemented an adjustment function -> still needs to obtain the new sensor values and need to double check whether the +- is correctly used
  * Added a new state "ADJUST"
  * Added a counter to the main script for the state "RUNNING" to ensure the robot stops after 3 turns
  * Robot physical adjustment: *Swtich the IR sensors to the right side of the robot while still facing the left *Change the sensor near the rear motor to long range sensor *Change the location of the gyro to the front incase of blocking the IR sensors


