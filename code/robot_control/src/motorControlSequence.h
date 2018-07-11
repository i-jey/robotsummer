#ifndef MOTORCONTROLSEQUENCE_H
#define MOTORCONTROLSEQUENCE_H
#include "motor.h"

class MotorControl { 
    private: 
        unsigned long delay; 
        int state; 
        Motor leftMotor; 
        Motor rightMotor; 
        int speed; 
        int reverse1Time; 
        int reverse2Time; 
        int bridge1WaitTime; 
        int bridge2WaitTime; 

        // Modifiable PID values
        int gain; 
        int pVal; 
        int iVal; 
        int dVal; 

    public: 
        MotorControl(Motor &leftMotor, Motor &rightMotor, int bridge1WaitTime, int bridge2WaitTime, 
                        int reverse1Time, int reverse2Time); 
        void reset(); 
        void poll(); 
        void stateOverride(int specialState); 
        void updateSpeed(int newSpeed); 
        void updateP(int newP); 
        void updateI(int newI); 
        void updateD(int newD); 
};

#endif