#include "includes.h" 
#include "motorControlSequence.h"

MotorControl::MotorControl(Motor &leftMotor, Motor &rightMotor, int bridge1WaitTime, int bridge2WaitTime, 
                            int reverse1Time, int reverse2Time) { 
    // FYI, the ampersand &, is a pointer (i.e as opposed to making a copy of the object
    // this means we will be referencing the actual object that was passed in)
    this->leftMotor = leftMotor; 
    this->rightMotor = rightMotor; 
    this->bridge1WaitTime = bridge1WaitTime; 
    this->bridge2WaitTime = bridge2WaitTime;
    this->reverse1Time = reverse1Time; 
    this->reverse2Time;
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

    if (millis() < delay) { 
        return; 
    }

    switch(state) { 
        case 0: 
            // This state is only meant for basic testing
            leftMotor.forward(speed); 
            rightMotor.forward(speed); 
            break; 
        case 1: 
            // TODO 
            // ayy lmao, write some PID functions, call them here
            break; 
        case 10: 
            // TODO 
            // Custom bridge dropping maneuver
            // Need to figure out how to stop reversing after a set time without
            // writing blocking code
            leftMotor.reverse(speed); 
            leftMotor.reverse(speed); 
        case 20: 
            // Stop motors until state changes (ie. from IR class?)
            leftMotor.forward(0); 
            rightMotor.forward(0); 
        case 30: 
            // TODO
            // Custom bridge dropping maneuver
        case 40: 
            // TODO 
            // Edge detection
            // damn this will be a doozy
        case 50: 
            // Once last ewok has been picked up, basket drop 
            // Reverse motors and raise basket in parallel, wait, drive forward
        default: 
            break; 
    }
}

void MotorControl::stateOverride(int specialState) { 
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
}

void MotorControl::updateSpeed(int newSpeed) { 
    speed = newSpeed; 
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