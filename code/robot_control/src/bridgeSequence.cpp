#include "includes.h"
#include "bridgeSequence.h"

bool onEdge = false; 

BridgeSequence::BridgeSequence(){};
BridgeSequence::BridgeSequence(Bridge &bridge, int bridge1Delay, int bridge2Delay, int rotateDelay) { 
    this->bridge = bridge; 
    this->bridge1Delay = bridge1Delay;
    this->bridge2Delay = bridge2Delay;
    this->rotateDelay = rotateDelay;
    this->angle = bridge.firstBridgeUpperAngle; 
}

void BridgeSequence::poll() { 
    if (millis() < delay) { 
        return; 
    }

    switch(state) { 
        case 0: 
            if (bridge.detectEdge() || onEdge) { 
                onEdge = true; 
                angle--; 
                bridge.lowerBridge1(angle); 
                delay = millis() + 50; 
                temp = 1; 
            }
            delay = millis() + 50; 
            
            if (angle == bridge.firstBridgeLowerAngle) { 
                onEdge = false;
                state++; 
                delay = millis() + bridge1Delay;
            }
            break; 
        case 1: 
            bridge.raiseBridge1(); 
            
            // angle = bridge.secondBridgeUpperAngle;
            // state++; 

            // TEMPORARY
            angle = bridge.firstBridgeUpperAngle;
            state = 0; 
            break;
        case 2: 
            if (bridge.detectEdge() || onEdge) { 
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

void BridgeSequence::updateDelayTime1(int newTime) { 
    this->bridge1Delay = newTime; 
}

void BridgeSequence::updateDelayTime2(int newTime) { 
    this->bridge2Delay = newTime; 
}

void BridgeSequence::updateRotateTime(int newTime) { 
    this->rotateDelay = newTime; 
}