

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

## 17 Mar
  * side correction controller stable
  * align function working 
  * 2X short range IR on left side
  * 3D printed initital model of ultrasonic
  * TO DO:
    * mount IR + Sam's code + test
    * cw function redo using gyro? IR?
    * make trashy `open loop` versions of movement functions
    * DIST_BETWEEN_IR needs to be measured in cm
    * SIDE_DIST_TARGET is currently 150 - 0 but zero needs to be replaced by the distance from IR sensors to centreline of robot (in cm)
    * ultrasonic target distance needs to be changed once the sensor is mounted
    * getting ir distances uses datasheet calculations, may need calibration
    * improve get_ultrasonic_range code
  * modified clockwise and counter-clockwise turning (by using angle calculations)
  * added IR sensor reading function in 
  
 ## 19 Mar
  * Sam added ultrasonic sensor code and also implemented gyro turning
	* align robot state after rotation?
	* Need to test and edit GYRO_TARGET_ANGLE and ULTRASONIC_MOVE_THRESH
	* maybe run gyro calibration after every turn?

## 22 Mar
  * ultrasonic works, just need calibrate
  * angle integrator static local variable remove count
    
	

  
  

  
  