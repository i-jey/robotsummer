#include "includes.h"
#include "clawSequence.h"

ClawSequence::ClawSequence(){};
ClawSequence::ClawSequence(Arm &arm, int closeTime, int raiseTime, int openTime, int closeTime2, int lowerTime, int resetTime) { 
    this->arm = arm; 
    this->closeTime = closeTime;
    this->raiseTime = raiseTime;
    this->openTime = openTime;
    this->closeTime2 = closeTime2;
    this->lowerTime = lowerTime;
    this->resetTime = resetTime; 

    state = 0; 
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
    if (millis() < delay) { 
        return; 
    }
    switch(state) { 
        case 0: 
            if (arm.ewokDetected()) { 
                arm.close(); 
                state++; 
                delay = millis() + closeTime; 
            }
            else { 
                arm.open(); 
                // Only read every 50ms, avoids false button reads
                delay = millis() + 50; 
            }
            break; 
        case 1: 
            arm.raise();
            state++; 
            delay = millis() + raiseTime; 
            break; 
        case 2: 
            arm.open(); 
            state++; 
            delay = millis() + openTime; 
            break; 
        case 3: 
            arm.close(); 
            state++; 
            delay = millis() + closeTime2; 
            break; 
        case 4: 
            arm.lower(); 
            state++; 
            delay = millis() + lowerTime; 
            break;
        case 5: 
            arm.open(); 
            reset(); 
            delay = millis() + resetTime; 
            break; 
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