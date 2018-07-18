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

        int defaultMotorSpeed; 
        int leftMotorSpeed; 
        int rightMotorSpeed; 

        int pidGain; 

    public: 
        TapeFollow(); 
        TapeFollow(int qrdLeft, int qrdRight); 
        void followTape(int qrdThreshold, int gain, int p, int i, int d); 
        int getLeftMotorSpeed(); 
        int getRightMotorSpeed(); 
}; 

#endif