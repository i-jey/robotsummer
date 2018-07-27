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
            if (bridge.detectEdge()) { 
                temp = 1; 
                delay = millis() + 1000; 
                state++; 
            }
            break; 
        case 1: 
            if (angle > bridge.firstBridgeLowerAngle) { 
                angle--; 
                bridge.lowerBridge1(angle); 
                delay = millis() + 25; 
            }
            else { 
                state++; 
                delay = millis() + 2500; 
            }
            break;
        case 2: 
            if (millis() > delay) { 
                bridge.raiseBridge1(); 
                state = 5; 
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