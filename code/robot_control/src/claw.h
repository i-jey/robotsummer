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
        void close(); 
        void open(); 
        void raise(); 
        void lower(); 
}; 

#endif 