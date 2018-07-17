#ifndef CLAW_H
#define CLAW_H

class Arm { 
    private: 
        Servo clampServo; 
        Servo armServo; 
        int pushButton; 

    public: 
        Arm();
        Arm(int clampPin, int armPin, int pushButton); 
        bool ewokDetected(); 
        bool close(); 
        bool open(); 
        bool raise(); 
        bool lower(); 
}; 

#endif 