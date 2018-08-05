#ifndef MOTORCONTROLSEQUENCE_H
#define MOTORCONTROLSEQUENCE_H

class MotorControl { 
    private: 
        unsigned long delay; 
        unsigned long specialStateDelay; 
        int state; 

        // Modules to keep track of / control
        Motor &leftMotor; 
        Motor &rightMotor; 
        Bridge &bridge;              // Crossing gaps
        IRReader &ir;                // Crossing IR gate
        ClawSequence &leftClaw;      // Override claw polling (i.e raise at gate)
        ClawSequence &rightClaw; 
        Basket &basket;              // Rotation and basket hooking 
        TapeFollow &pidControl; 
        
        // Default variables 
        int defaultSpeed; 
        int speedLeft;
        int speedRight;

        // PID
        int qrdThreshold; 
        int gain; 
        int pVal; 
        int iVal; 
        int dVal; 

        // bridge 
        int edgeThreshold; 
        int angle; 

        // Local convenience functions
        void updateSpeedLeft(int newSpeed);
        void updateSpeedRight(int newSpeed);
        void continuousForward(); 
        void continuousReverse(); 
        void pid(); 
        void rotateLeft(); 
        void rotateRight(); 
        void rotateLeftJolt(); 
        void rotateRightJolt();

    public: 
        MotorControl(Motor &leftMotor, Motor &rightMotor, Bridge &bridge, IRReader &ir, 
            Basket &basket, TapeFollow &pidControl, ClawSequence &leftClaw, ClawSequence &rightClaw, 
            int qrdThreshold, int gain, int p, int i, int d);
        void begin(); 
        void reset(); 
        void specialStateChecker(); 
        void poll(); 
        void stateOverride(int specialState, int delay); 

        void updateDefaultSpeed(int newSpeed); 
        void updateSpeed(int newSpeed); 

        void updateThreshold(int newThreshold); 
        void updateGain(int newGain); 
        void updateP(int newP); 
        void updateI(int newI); 
        void updateD(int newD); 

        void updateEdgeThreshold(int newThreshold); 

        // First bridge sequence variables 
        int edgeReverseDistance = 0; 
        int dropBridgeDistance = 0; 
        int driveOverDistance = 0; 

        // Drop ewok sequence 
        int s3TiltLeftTime = 100; 
        int s3ReverseTime = 250; 
        int dropEwokTime = 1000; 
        int s3LeftPullBackTime = 250; 
        int bias; 
};

#endif
