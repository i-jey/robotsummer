#ifndef TAPEFOLLOW_H 
#define TAPEFOLLOW_H 

class TapeFollow { 
    private: 
        int leftQRDPin; 
        int rightQRDPin; 
        bool leftQRDState = false; 
        bool rightQRDState = false; 
        int error = 0; 
        int lastError = 0; 

        int defaultMotorSpeed = 0; 
        int leftMotorSpeed = 0; 
        int rightMotorSpeed = 0; 

        int pidGain; 
        int qrdThreshold; 

    public: 
        TapeFollow(); 
        TapeFollow(int qrdLeft, int qrdRight); 
        void followTape(int qrdThreshold, int gain, int p, int i, int d, int defaultMotorSpeed); 
        int getLeftMotorSpeed(); 
        int getRightMotorSpeed(); 
        int getLeftQRDReading(); 
        int getRightQRDReading(); 
        bool leftOnTape(); 
        bool rightOnTape(); 
        void updateSpeed(int newSpeed); 
}; 

#endif