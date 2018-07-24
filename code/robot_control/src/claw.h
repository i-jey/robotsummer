#ifndef CLAW_H
#define CLAW_H

class Arm { 
    private: 
        Servo clampServo; 
        Servo armServo; 
        int pushButton; 
        int closeAngle; 
        int openAngle; 
        int openAngleInside; 
        int lowerAngle; 
        int raiseAngle; 

    public: 
        Arm();
        Arm(int clampPin, int armPin, int pushButton, int closeAngle, int openAngle, int openAngleInside, int lowerAngle, int raiseAngle); 
        bool ewokDetected(); 
        void close(); 
        void open(bool isInside); 
        void raise(); 
        void lower(); 
}; 

#endif 