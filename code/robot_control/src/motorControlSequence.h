#ifndef MOTORCONTROLSEQUENCE_H
#define MOTORCONTROLSEQUENCE_H
#include "motor.h"
#include "tapeFollow.h" 

class MotorControl { 
    private: 
        unsigned long delay; 
        int state; 

        Motor leftMotor; 
        Motor rightMotor; 

        int defaultSpeed; 
        int speedLeft;
        int speedRight;

        int reverseTime1; 
        int reverseTime2; 

        int bridge1WaitTime; 
        int bridge2WaitTime; 

        int forwardDriveTime1; 
        int forwardDriveTime2; 

        int leftRotate90Delay; 
        int rightRotate90Delay; 

        // PID
        TapeFollow pidControl; 
        int qrdThreshold; 
        int gain; 
        int pVal; 
        int iVal; 
        int dVal; 

        // Local convenience functions
        void updateSpeedLeft(int newSpeed);
        void updateSpeedRight(int newSpeed);
        void continuousForward(); 
        void continuousReverse(); 
        void pid(); 
        void rotateLeft(); 
        void rotateRight(); 

    public: 
        MotorControl();
        MotorControl(int startingState, int startingSpeed, Motor &leftMotor, Motor &rightMotor, 
                TapeFollow &pidControl, int qrdThreshold, int gain, int p, int i, int d, int reverse1Time, int reverse2Time, 
                int bridge1WaitTime, int bridge2WaitTime, int forwardDriveTime1, int forwardDriveTime2, int rotateLeftDelay, int rotateRightDelay);

        void reset(); 
        void poll(); 
        void stateOverride(int specialState, int delay); 

        void updateDefaultSpeed(int newSpeed); 
        void updateSpeed(int newSpeed); 

        void updateThreshold(int newThreshold); 
        void updateGain(int newGain); 
        void updateP(int newP); 
        void updateI(int newI); 
        void updateD(int newD); 
        void updateReverseTime1(int newTime); 
        void updateDropBridge1Time(int newTime); 
        void updateForwardDrive1(int newTime); 
};

#endif
