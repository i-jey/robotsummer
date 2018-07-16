#ifndef MOTORCONTROLSEQUENCE_H
#define MOTORCONTROLSEQUENCE_H
#include "motor.h"

class MotorControl { 
    private: 
        unsigned long delay; 
        int state; 
        Motor leftMotor; 
        Motor rightMotor; 
        int speedLeft;
        int speedRight;
        int reverseTime1; 
        int reverseTime2; 
        int bridge1WaitTime; 
        int bridge2WaitTime; 
        int forwardDriveTime1; 
        int forwardDriveTime2; 

        // Modifiable PID values
        int gain; 
        int pVal; 
        int iVal; 
        int dVal; 

    public: 
        MotorControl();
        MotorControl(int startingState, int startingSpeed, Motor &leftMotor, Motor &rightMotor, 
                int reverse1Time, int reverse2Time, int bridge1WaitTime, int bridge2WaitTime,
                int forwardDriveTime1, int forwardDriveTime2); 
        void reset(); 
        void poll(); 
        void stateOverride(int specialState, int delay); 
        void updateSpeed(int newSpeed); 
        void updateSpeedLeft(int newSpeed);
        void updateSpeedRight(int newSpeed);
        void updateGain(int newGain); 
        void updateP(int newP); 
        void updateI(int newI); 
        void updateD(int newD); 
};

#endif
