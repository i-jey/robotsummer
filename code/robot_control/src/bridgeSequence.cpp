#include "includes.h"
#include "bridgeSequence.h"

BridgeSequence::BridgeSequence(Bridge &bridge, int bridge1Delay, int bridge2Delay, int rotateDelay) { 
    this->bridge = bridge; 
    this->bridge1Delay = bridge1Delay;
    this->bridge2Delay = bridge2Delay;
    this->rotateDelay = rotateDelay;
    angle = 0; 
}

void BridgeSequence::poll() { 
    if (millis() < delay) { 
        return; 
    }

    switch(state) { 
        case 0: 
            if (bridge.detectEdge()) { 
                angle++; 
                bridge.lowerBridge1(angle); 
                delay = millis() + 10; 
                temp = 1; 
            }
            // change to variable in a sec
            if (angle == 47) { 
                state++; 
                delay = millis() + bridge1Delay;
            }
            delay = millis() + 50; 
            break; 
        case 1: 
            bridge.raiseBridge1(); 
            state++; 
            break;
        case 2: 
            if (bridge.detectEdge()) { 
                // second edge detected 
                // add global flag to start custom movement and edge navigation
                delay = millis() + rotateDelay;
            }
            break; 
        case 3: 
            if (bridge.lowerBoth()) { 
                state++; 
                delay = millis() + bridge2Delay; 
            }
            delay = millis() + 50; 
            break; 
        case 4: 
            bridge.raiseBoth(); 
            state++; 
            break;
        default: 
            break; 
    }
}