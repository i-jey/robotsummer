#include "includes.h" 
#include "motorControlSequence.h"

MotorControl::MotorControl(){};
MotorControl::MotorControl(int startingState, int startingSpeed, Motor &leftMotor, Motor &rightMotor, 
    TapeFollow &pidControl, int qrdThreshold, int gain, int p, int i, int d, int reverseTime1, int reverseTime2, 
    int bridge1WaitTime, int bridge2WaitTime, int forwardDriveTime1, int forwardDriveTime2) { 

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
   Serial.print("Speed: "); Serial.println(speedLeft); 
    switch(state) { 
        case 0: 
            // This state is only meant for basic testing
            leftMotor.forward(speedLeft); 
            rightMotor.forward(speedRight); 
            break; 
        case 1: 
            // This state is only meant for basic testing 
            leftMotor.reverse(speedLeft); 
            rightMotor.reverse(speedRight); 
            break; 
        case 2:
            // Tape following 
            pidControl.followTape(qrdThreshold, gain, pVal, iVal, dVal, defaultSpeed); 

            updateSpeedLeft(pidControl.getLeftMotorSpeed()); 
            updateSpeedRight(pidControl.getRightMotorSpeed()); 

            leftMotor.forward(speedLeft); 
            rightMotor.forward(speedRight); 
            
            break; 
        case 10: 
            // Edge detected, stop
            leftMotor.reverse(255); 
            leftMotor.reverse(255);
            if (millis() > delay) { 
                state++; 
                delay = millis() + bridge1WaitTime; 
            } 
            break; 
        case 11: 
            // wait for bridge to lower
            leftMotor.forward(0); 
            rightMotor.forward(0); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + reverseTime1; 
            }
            break;
        case 12: 
            // Reverse to drop bridge 
            leftMotor.reverse(speedLeft); 
            leftMotor.reverse(speedRight); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + forwardDriveTime1; 
            }
            break; 
        case 13: 
            // CHANGE THIS later
            leftMotor.forward(150); 
            rightMotor.forward(150);
            if (millis() > delay) { 
                // CHANGE THE DEFAULT STATE, 0 for testing for now
                state = 0; 
            } 
            break;
        case 20: 
            // Stop motors until state changes (ie. from IR class?)
            leftMotor.forward(0); 
            rightMotor.forward(0); 
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
   this->delay = delay; 
}

void MotorControl::updateDefaultSpeed(int newSpeed) { 
    this->defaultSpeed = newSpeed; 
}

void MotorControl::updateSpeed(int newSpeed) { 
    this->speedLeft = newSpeed; 
    this->speedRight = newSpeed;
}

void MotorControl::updateSpeedLeft(int newSpeed) {
    speedLeft = newSpeed;
}

void MotorControl::updateSpeedRight(int newSpeed) {
    speedRight = newSpeed;
}

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





