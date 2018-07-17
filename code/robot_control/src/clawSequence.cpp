#include "includes.h"
#include "clawSequence.h"

ClawSequence::ClawSequence(){};
ClawSequence::ClawSequence(Arm &arm, int raiseDelay, int openDelay, int lowerDelay) { 
    this->arm = arm; 
    this->raiseDelay = raiseDelay; 
    this->openDelay = openDelay;
    this->lowerDelay = lowerDelay;
}

void ClawSequence::reset() { 
    state = 0; 
    delay = millis(); 
}

void ClawSequence::poll() { 
    /* Define claw states
    *  0 : Polling push buttons, advances to state=1 after raiseDelay if button detected.
    *  1 : Raises arm, advances to state=2 after raiseDelay
    *  2 : Opens arm (to drop Browok in basket), advances to state=3 after lowerDelay
    *  3 : Lowers arm, resets to state=0 (polling push buttons)
    */
    Serial.print("State: "); Serial.println(state); 
    if (millis() < delay) { 
        return; 
    }

    switch(state) { 
        case 0: 
            if (arm.close()) { 
                state++; 
                delay = millis() + raiseDelay; 
            }
            else { 
                arm.open(); 
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
        case 10:
            arm.raise();  
            break; 
        default: 
            break; 
    }; 
}

void ClawSequence::stateOverride(int specialState) {
    /*  Manual state override for special sequences 
    *   Should be used with care. Allows us to specify unique claw behaviour.
    *   Define unique states: 
    *   10 : Raise arm and keep arm raised while in this state
    */
    state = specialState;
}