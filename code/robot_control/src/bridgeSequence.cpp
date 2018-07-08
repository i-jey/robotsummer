#include "includes.h"
#include "bridgeSequence.h"

BridgeSequence::BridgeSequence(Bridge &bridge, int bridge1Delay, int bridge2Delay, int rotateDelay) { 
    bridge = bridge; 
    bridge1Delay = bridge1Delay;
    bridge2Delay = bridge2Delay;
    rotateDelay = rotateDelay;
}

void BridgeSequence::poll() { 
    if (millis() < delay) { 
        return; 
    }

    switch(state) { 
        case 0: 
            if (bridge.lowerBridge1()) { 
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