#ifndef CLAWSEQUENCE_H
#define CLAWSEQUENCE_H
#include "claw.h"

class ClawSequence {
    private: 
        unsigned long delay; 
        int closeTime; 
        int raiseTime; 
        int openTime; 
        int closeTime2; 
        int lowerTime; 
        int resetTime;  
        int state; 
        Arm arm; 

    public: 
        ClawSequence(); 
        ClawSequence(Arm &arm, int closeTime, int raiseTime, int openTime, int closeTime2, int lowerTime, int resetTime); 
        void reset(); 
        void poll(); 
        void stateOverride(int specialState); 
};

#endif