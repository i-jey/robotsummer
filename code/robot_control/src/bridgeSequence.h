#ifndef BRIDGESEQUENCE_H
#define BRIDGESEQUENCE_H
#include "bridge.h"

class BridgeSequence { 
    private: 
        unsigned long delay = 0; 
        int state; 
        int bridge1Delay; 
        int bridge2Delay; 
        int rotateDelay; 
        int angle; 
        Bridge bridge; 

    public: 
        BridgeSequence(Bridge &bridge, int bridge1Delay, int bridge2Delay, int rotateDelay); 
        void reset(); 
        void poll(); 
}; 

#endif