#ifndef CLAW_H
#define CLAW_H

class Arm { 
    private: 
        Servo clampServo; 
        Servo armServo; 

        int armPin; 
        int clampPin; 
        int triggerPin; 
        int closeAngle; 
        int openAngle; 
        int openAngleInside; 
        int lowerAngle; 
        int raiseAngle; 
        int verticalAngle; 
        int phoenixAngle;

    public: 
        Arm();
        Arm(int clampPin, int armPin, int triggerPin, int closeAngle, int openAngle, int openAngleInside, int lowerAngle, int raiseAngle, int verticalAngle, int phoenixAngle); 
        void begin(); 
        bool ewokDetected(); 
        void close(); 
        void open(bool isInside); 
        void raise(); 
        void verticalRaise(); 
        void lower(); 
        void customAngle(int angle); 
        void phoenix(); 
}; 

#endif 