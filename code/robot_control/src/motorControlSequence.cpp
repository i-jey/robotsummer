#include "includes.h" 

MotorControl::MotorControl(){};
MotorControl::MotorControl(int startingState, int startingSpeed, Motor &leftMotor, Motor &rightMotor, 
    TapeFollow &pidControl, int qrdThreshold, int gain, int p, int i, int d, int reverseTime1, int reverseTime2, 
    int bridge1WaitTime, int bridge2WaitTime, int forwardDriveTime1, int forwardDriveTime2, 
    int leftRotate90Delay, int rightRotate90Delay) { 

    // FYI, the ampersand &, is a pointer (i.e as opposed to making a copy of the object
    // this means we will be referencing the actual object that was passed in)
    this->state = startingState;
    this->defaultSpeed = startingSpeed;  
    this->speedLeft = startingSpeed;
    this->speedRight = startingSpeed;

    this->leftMotor = leftMotor; 
    this->rightMotor = rightMotor; 

    this->reverseTime1 = reverseTime1; 
    this->reverseTime2 = reverseTime2;

    this->bridge1WaitTime = bridge1WaitTime; 
    this->bridge2WaitTime = bridge2WaitTime;

    this->forwardDriveTime1 = forwardDriveTime1; 
    this->forwardDriveTime2 = forwardDriveTime2; 

    this->leftRotate90Delay = leftRotate90Delay; 
    this->rightRotate90Delay = rightRotate90Delay; 

    this->qrdThreshold = qrdThreshold;  
    this->gain = gain; 
    this->pVal = p; 
    this->iVal = i; 
    this->dVal = d; 

    this->pidControl = pidControl; 
}

void MotorControl::reset() { 
    state = 0; 

    // For now, I can't foresee where we may want to add 
    // delays between switching between motor logic, but it's here
    // in case the need arises    
    delay = millis(); 
}

void MotorControl::poll() { 
    /*  Define motor states
    *   0 : Continuous forward drive at a desired speed 
    *   1 : Tape following w/ PID 
    *   10 : Bridge deployment 1 
    *   20 : Infrared sensor wait
    *   30 : Bridge deployment 2 
    *   40 : Edge detection motor control 
    *   50 : Basket sequence
    */  
    Serial.print("L: "); Serial.println(speedLeft); 
    Serial.print("R: "); Serial.println(speedRight); 
    // Serial.print("QRD L: "); Serial.println(pidControl.getLeftQRDReading());
    // Serial.print("QRD R: "); Serial.println(pidControl.getRightQRDReading());
    switch(state) { 
        /** 
         * Cases:
         * 0 - 4, basic functions for testing
         * 5-9, pid and get first ewok, turn back
         * 10-13, first bridge deployment and crossing
         **/
        case 0:  
            continuousForward(); 
            break; 
        case 1: 
            continuousReverse();
            break; 
        case 2:
            // Tape following
            pid(); 
            break; 
        case 3: 
            rotateLeft();  

            if (millis() > delay) { 
                state = 20; 
            }
            break; 
        case 4: 
            rotateRight();

            if (millis() > delay) { 
                state = 20; 
            }
            break;
        case 5: 
            pid(); 
            if (firstEwok) { 
                delay = millis() + 500; 
                state++; 
            }
            break; 
        case 6: 
            // Rotate 180 degree to go back home
            rotateLeft(); 
            if (millis() > delay) { 
                delay = millis() + 500; 
                state++; 
            }
            break; 
        case 7: 
            // Rotation complete, try to find tape again

            // Stop sweep once 180 time is up or left qrd is on tape
            if (pidControl.getLeftQRDReading()) { 
                state+=2; 
            }
            else if (millis() > delay) { 
                delay = millis() + 500; 
                state++; 
            }
            // Sweep left
            else { 
                rotateLeft(); 
            }
            break; 
        case 8: 
            // Stop sweep once 180 time is up or right qrd is on tape 
            if (pidControl.getRightQRDReading()) { 
                state++; 
            }
            else if (millis() > delay) { 
                delay = millis() + 1000; 
                state++; 
            }
            // Sweep right 
            else {      
                rotateRight();
            }
            break; 
        case 9: 
            pid(); 
            break; 
        case 10: 
            // Edge detected, reverse
            continuousReverse();

            // UNCOMMENT this if continuousreverse isn't working
            // leftMotor.write(-80); 
            // rightMotor.write(-80); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + bridge1WaitTime; 
            } 
            break; 
        case 11: 
            // wait for bridge to lower
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + reverseTime1; 
            }
            break;
        case 12: 
            // Reverse to drop bridge 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + forwardDriveTime1; 
            }
            break; 
        case 13: 
            // CHANGE THIS later
            leftMotor.write(defaultSpeed); 
            rightMotor.write(defaultSpeed);
            if (millis() > delay) { 
                // CHANGE THE DEFAULT STATE, 20 for testing for now
                state = 20; 
            } 
            break;
        case 20: 
            // Stop motors until state changes (ie. from IR class?)
            leftMotor.write(0); 
            rightMotor.write(0); 
            break; 
        case 30: 
            // TODO
            // Custom bridge dropping maneuver
            break; 
        case 40: 
            // TODO 
            // Edge detection
            // damn this will be a doozy
            break; 
        case 50: 
            // Once last ewok has been picked up, basket drop 
            // Reverse motors and raise basket in parallel, wait, drive forward
            break; 
        default: 
            break; 
    }
}

void MotorControl::stateOverride(int specialState, int delay) { 
    /*  Manual state override for special motor sequences 
    *   Again, use with care, can get diffcult to debug if we flip flop states like my boy Willy Wonka
    *   Define unique motor states: 
    *   10 : First bridge deployment (reverse, wait, forward)
    *   20 : Infrared wait
    *   30 : Second bridge deployment (reverse, rotate 90 right, wait, forward) 
    *   40 : Edge detection algorithm
    *   50 : Basket sequence
    */
   state = specialState;
   this->delay = millis() + delay; 
}

/** 
 * Takes in an integer speed to be used as the new default speed. 
 * The default speed sets the base speed for PID control.
**/
void MotorControl::updateDefaultSpeed(int newSpeed) { 
    this->defaultSpeed = newSpeed; 
}

/**
 * Takes in an integer
 * Sets the current speeds being written to the motors.
**/
void MotorControl::updateSpeed(int newSpeed) { 
    this->speedLeft = newSpeed; 
    this->speedRight = newSpeed;
}

/** 
 * Takes in an integer 
 * Updates the speed of the left motor
**/ 
void MotorControl::updateSpeedLeft(int newSpeed) {
    speedLeft = newSpeed;
}

/** 
 * Takes in an integer
 * Updates the speed of the right motor
**/ 
void MotorControl::updateSpeedRight(int newSpeed) {
    speedRight = newSpeed;
}

/**
 * Takes in an integer
 * Updates the value of the QRD threshold (used to determine whether
 * on tape or not in PID control) 
**/ 
void MotorControl::updateThreshold(int newThreshold) { 
    qrdThreshold = newThreshold; 
}

void MotorControl::updateGain(int newGain) { 
    gain = newGain; 
}

void MotorControl::updateP(int newP) { 
    pVal = newP; 
}

void MotorControl::updateI(int newI) { 
    iVal = newI; 
}

void MotorControl::updateD(int newD) { 
    dVal = newD; 
}

//// High level motor control functions //// 

void MotorControl::continuousForward() { 
    leftMotor.write(defaultSpeed); 
    rightMotor.write(defaultSpeed); 
}

void MotorControl::continuousReverse() { 
    leftMotor.write(-defaultSpeed); 
    rightMotor.write(-defaultSpeed); 
}   

void MotorControl::pid() { 
    pidControl.followTape(qrdThreshold, gain, pVal, iVal, dVal, defaultSpeed); 

    updateSpeedLeft(pidControl.getLeftMotorSpeed()); 
    updateSpeedRight(pidControl.getRightMotorSpeed()); 
    
    leftMotor.write(speedLeft); 
    rightMotor.write(speedRight); 
}

void MotorControl::rotateLeft() { 
    leftMotor.write(-defaultSpeed); 
    rightMotor.write(defaultSpeed);
}

void MotorControl::rotateRight() { 
    leftMotor.write(defaultSpeed); 
    rightMotor.write(-defaultSpeed);
}