#include <kipr/wombat.h>
#include <stdio.h>
 
//motor Ports
#define mRIGHT 1
#define mLEFT 0
 
//analog Ports
#define aMIDDLE 1
#define aRIGHT 3
#define aLEFT 2
#define aDISTANCE 0
 
//servo Ports
#define CLAW 1
#define ARM 0
 
//servo positions
#define CLAW_OPEN 0
#define CLAW_CLOSE 1000
#define ARM_DOWN 1900
#define ARM_UP 870
 
//analog value for the black line, will change if the light is different
#define BLACK 3400
//min analog Value for the distance sensor
#define DISTANCE 2900
//motor speed
#define SPEED -50
//the time before the claw is closed, important in PICKUP() function
#define CLAW_WAIT_TIME 1105
 
/**
* @brief checks if a sensor is above the black line
* 
* @param sensor the port of an analog sensor
* @return returns 1 if the sensor detects black else returns 0
*/
int onLine(int sensor) {
    if(analog(sensor) > BLACK) {
        return 1;
    }
    return 0;
}
 
/**
* @brief simulare to onLine() function but you can enter the minimum Value of the analog sensor
* 
* @param sensor the port of an analog sensor
* @param min the minimum Value of the analog sensor
* @return returns 1 if the sensor value is above the min else returns 0
*/
int isObjectInRange(int sensor, int min) {
    if(analog(sensor) > min) {
        return 1;
    }
    return 0;
}
 
/**
* @brief makes the right and left motor move for CLAW_WAIT_TIME seconds and then closes the Claw and lifts the arm up
*        if the value of the inputed pointer is 0
* 
* @param *n pointer of a variable that will be set to 1 after the code has been exectuted
*/
void PICKUP(int *n) {
    if(*n == 0) {
        motor(mLEFT, SPEED);
    	motor(mRIGHT, SPEED);
    	msleep(CLAW_WAIT_TIME);
    	ao();
    	set_servo_position(CLAW, CLAW_CLOSE);
    	msleep(1000);
    	set_servo_position(ARM, ARM_UP);
    	msleep(1000);
        *n = 1;
    }
}
 
/**
* @brief adds 1 to a variable if n = 1
* 
* @param n if n = 1 the variable will increase by 1
* @param *var the pointer to a variable that will be changed
*/
void count(int n, int *var) {
    if(n == 1) {
        (*var)++;
    }
}
 
int main() {
    //setup servos
    enable_servos();
    set_servo_position(ARM, ARM_DOWN);
    set_servo_position(CLAW, CLAW_OPEN);
    // hasPickedUp will be changed to 1 if the target has been picked up
    int hasPickedUp = 0;
    int loopCount = 0;
 
    while(1) {
        if(isObjectInRange(aDISTANCE, DISTANCE)){
            PICKUP(&hasPickedUp);
        }
 
        //controlls motors
     	if(onLine(aMIDDLE)) {
            motor(mLEFT, SPEED);
        	motor(mRIGHT, SPEED);
        } else if(onLine(aRIGHT)) {
            motor(mRIGHT, SPEED);
            motor(mLEFT, 0);
        } else if(onLine(aLEFT)) {
            motor(mLEFT, SPEED);
            motor(mRIGHT, 0);
        } else {
            motor(mLEFT, SPEED);
        	motor(mRIGHT, SPEED);
        }
        //start to count if target has been picked up
        count(hasPickedUp, &loopCount);
        //break after a couple of seconde while holding the target, this should be changed
        if(loopCount > 5000) {
            break;
        }
    }
    return 0;
}