#ifndef CLAWSEQUENCE_H
#define CLAWSEQUENCE_H
#include "claw.h"

class ClawSequence {
    private: 
        unsigned long delay; 
        int raiseDelay; 
        int openDelay; 
        int lowerDelay; 
        int state; 
        Arm arm; 

    public: 
        ClawSequence(); 
        ClawSequence(Arm &arm, int raiseDelay, int openDelay, int lowerDelay); 
        void reset(); 
        void poll(); 
        void stateOverride(int specialState); 
};

#endif