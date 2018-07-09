#include "includes.h"
#include "clawSequence.h"

ClawSequence::ClawSequence(Arm &arm, int raiseDelay, int openDelay, int lowerDelay) { 
    this->arm = arm; 
    this->raiseDelay = raiseDelay; 
    this->openDelay = openDelay;
    this->lowerDelay = lowerDelay;
}

void ClawSequence::reset() { 
    state = 0; 

    // Poll button every 50ms
    delay = millis(); 
}

void ClawSequence::poll() { 
    if (millis() < delay) { 
        return; 
    }

    switch(state) { 
        case 0: 
            if (arm.close()) { 
                state++; 
                delay = millis() + raiseDelay; 
            }
            // Only read every 50ms, avoids false button reads
            delay = millis() + 50; 
            break; 
        case 1: 
            if (arm.raise()) { 
                state++; 
                delay = millis() + openDelay; 
            }
            break; 
        case 2: 
            if (arm.open()) { 
                state++; 
                delay = millis() + lowerDelay; 
            }
            break; 
        case 3: 
            if (arm.lower()) { 
                reset(); 
            }
        default: 
            break; 
    }; 
}