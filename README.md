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
  
  
## 17 Mar
   * modified clockwise and counter-clockwise turning (by using angle calculations)
   * added IR sensor reading function in Motor_Movements
   * DIST_BETWEEN_IR needs to be measured in cm
   * SIDE_DIST_TARGET is currently 150 - 0 but zero needs to be replaced by the distance from IR sensors to centreline of robot (in cm)
   * ultrasonic target distance needs to be changed once the sensor is mounted

