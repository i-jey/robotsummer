#include "includes.h" 

// flags 
bool firstBridge = true; 
bool firstBridgeSequenceDone = false; 
bool throughGate = false; 
bool beforeStormTroopers = true; 
bool afterStormTroopers = true; 
bool detected1k = false; 
bool irGo = false; 
bool clawStartWait = true; 
bool clawBeforeGate = true; 
bool afterThirdEwok = true; 
bool ohBabyADouble = false; 
int reducedSpeed;
int nextState = 0; 
unsigned long nextDelay = 0; 

#define CLAW_POLL 0
#define CLAW_UP 10
#define CLAW_DOWN 11
#define CLAW_PHOENIX 12
#define CLAW_IN 13

MotorControl::MotorControl(Motor &leftMotor, Motor &rightMotor, Bridge &bridge, IRReader &ir, 
    Basket &basket, TapeFollow &pidControl, ClawSequence &leftClaw, ClawSequence &rightClaw, int qrdThreshold, int gain, int p, int i, int d) 
: leftMotor(leftMotor), rightMotor(rightMotor), bridge(bridge), ir(ir), basket(basket), pidControl(pidControl), leftClaw(leftClaw), rightClaw(rightClaw)
{ 
    this->qrdThreshold = qrdThreshold;  
    this->gain = gain; 
    this->pVal = p; 
    this->iVal = i; 
    this->dVal = d; 

    // Initialize bridge angle 
    // bridge.firstBridgeLowerAngle = 40;
    // bridge.firstBridgeUpperAngle = 130; 

    reducedSpeed = 230; // no need for this for now
}

void MotorControl::begin() { 
    leftMotor.begin(); 
    rightMotor.begin(); 
    bridge.begin(); 
    basket.begin(); 
    ir.begin(); 
    pidControl.begin(); 
    leftClaw.begin(); 
    rightClaw.begin(); 
}

void MotorControl::reset() { 
    firstBridge = true; 
    firstBridgeSequenceDone = false; 
    throughGate = false; 
    beforeStormTroopers = true; 
    detected1k = false; 
    irGo = false; 
    clawStartWait = true; 
    ohBabyADouble = false; 
    afterStormTroopers = true; 
    clawBeforeGate = true;
    afterThirdEwok = true; 
}

void MotorControl::specialStateChecker() {
    // Special state delay is used to prevent repeating a state (i.e restarting first bridge sequence multiple times as
    // it pulls away from the edge)
    
    if (millis() < specialStateDelay) { 
        return; 
    }

    // Switch to first bridge sequence
    if (ewokCounter == 1 && firstBridge) { 
        state = 7;
        delay = millis() + 20;  
        firstBridge = false; 
    }

    // IR special sequences  
    if (firstBridgeSequenceDone == true) { 
        if (detected1k == false) { 
            // Switch to wait state
            state = 100;  
            if (ir.read1k()) {detected1k = true;}
            leftClaw.stateOverride(CLAW_UP);
        }
        else if ((ir.read10k() && !ir.read1k()) && detected1k == true) { 
            if (irGo == false) { 
                state = 2; 
                irGo = true; 
            }
        }
    }

    if (ewokCounter > prevEwokCounter) { 
        nextState = state; 
        delay = millis() + 500; 
        nextDelay = 0; 
        state = 101; 
        prevEwokCounter = ewokCounter; 
    }
    if (ewokCounter == 2 && clawBeforeGate) { 
        specialStateDelay = millis() + 1350; 
        clawBeforeGate = false;  
    }
    if (ewokCounter == 2 && !throughGate) { 
        // Second ewok has been picked and dropped, put hands up 
        // Note the special delay from above is to prevent the claw from closing
        // before it has dropped the ewok

        // Freeze, both hands up! oh god please don't shoot please im too yung
        if (millis() > specialStateDelay) { 
            leftClaw.stateOverride(CLAW_UP); 
            rightClaw.stateOverride(CLAW_UP); 
             
            throughGate = true;
            specialStateDelay = millis() + 3250; // delay to get from gate to after storm troopers before bringing left claw down 
        }
    }
    if (millis() > specialStateDelay && ewokCounter == 2 && beforeStormTroopers && throughGate) { 
        // We've passed Stormy Daniels, bring left claw down (but wait before polling to prevent it triggering itself)
        leftClaw.stateOverride(11); 
        rightClaw.stateOverride(10); 

        beforeStormTroopers = false; 
        specialStateDelay = millis() + 350; 
    }
    if (millis() > specialStateDelay && !beforeStormTroopers && clawStartWait) { 
        // Start left claw polling
        leftClaw.stateOverride(CLAW_POLL); 
        clawStartWait = false; 
    }

    if (ewokCounter == 3 && afterStormTroopers) { 
        // Sequence to switch to after third ewok
        state = 39; 
        afterStormTroopers = false; 
        // delay = millis() + 550;
        delay = millis() + 500; 
        specialStateDelay = millis() + 1350; 
    }
    if (ewokCounter == 3 && afterThirdEwok && !afterStormTroopers) { 
        // Bring left claw back up after ewok has been picked and dropped
        if (millis() > specialStateDelay) { 
            leftClaw.stateOverride(CLAW_UP); 
            afterThirdEwok = false; 
        }
    }
}

void MotorControl::poll() { 
    specialStateChecker();
    globalMotorStateTracker = state; 

    switch(state) { 
        case 0: 
            continuousForward(); 
            break; 
        case 1: 
            continuousReverse();
            break; 
        case 2: 
            pid(); 
            break; 
        case 3: 
            rotateLeft();  

            if (millis() > delay) { 
                state = 100; 
            }
            break; 
        case 4: 
            rotateRight();

            if (millis() > delay) { 
                state = 100; 
            }
            break;
        case 5: 
            pid(); 
            if (ewokCounter == 1) {  
                state++; 
                delay = millis() + 2500;
            }
            break; 
        case 6: 
            leftMotor.write(0);
            rightMotor.write(0);
            leftClaw.stateOverride(10); 
            rightClaw.stateOverride(10); 
            if (millis() > delay) {
                state++;
                delay = millis() + 800; 
            }
            break; 
        // FIRST BRIDGE SEQUENCE
        case 7: 
            leftMotor.write(-255); 
            rightMotor.write(0); 

            if (bridge.detectRightEdge()) { 
                state++; 
                delay = millis() + 20; 
            }
            break; 
        case 8:
            // Temporary right jolt to stop
            leftMotor.write(255); 
            rightMotor.write(0); 

            if (millis() > delay) { 
                state++; 
            }
            break; 
        case 9: 
            // Drive forward till edge detected 
            leftMotor.write(200); 
            rightMotor.write(200); 

            if (bridge.detectLeftEdge()) { 
                state++; 
                delay = millis() + 30; 
            }
            break; 
        case 10: 
            // Over edge, jolt back
            leftMotor.write(-255); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                state++;
                delay = millis() + edgeReverseDistance;  
            }
            break; 
        case 11: 
            // Reverse 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed); 

            if (millis() > delay) { 
                state++;
                delay = millis() + 30;   
                angle = bridge.firstBridgeUpperAngle;
            }
            break; 
        case 12: 
            // Forward jolt 
            leftMotor.write(255); 
            rightMotor.write(255); 

            if (millis() > delay) { 
                state++; 
            }
            break; 
        case 13: 
            // Stop motors 
            leftMotor.write(0); 
            rightMotor.write(0); 

            if (millis() > delay) { 
                // Lower bridge 
                bridge.lowerBridge1(angle); 
                angle -= 10;
                delay = millis() + 15; 
            }
        
            if (angle < bridge.firstBridgeLowerAngle) { 
                state++;
                delay = millis() + dropBridgeDistance; 
            }

            break; 
        case 14: 
            // Reverse to drop bridge 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed); 
            
            if (millis() > delay) { 
                state++; 
                delay = millis() + 30; 
                bridge.raiseBoth(); 
            }
            break; 
        case 15: 
            leftMotor.write(255); 
            rightMotor.write(255); 
            
            if (millis() > delay) { 
                state++; 
            }
            break; 
        case 16: 
            // Drive over bridge
            leftMotor.write(defaultSpeed); 
            rightMotor.write(defaultSpeed); 

            if (pidControl.leftOnTape() || pidControl.rightOnTape()) { 
                state = 100; 
            }
            break; 
        case 17: 
            // Stop motors 
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) { 
                state++; 
                bridge.raiseBridge1(); 
                delay = millis() + driveOverDistance;
            }
            break; 
        case 18: 
            // Drive over bridge
            leftMotor.write(150 + 10); 
            rightMotor.write(150 - 5); 

            if (millis() > delay) { 
                nextState = ++state;
                nextDelay = 1000;  
                state = 101; 
            }

            break; 
        case 19: 
            // Sweep for tape 
            leftMotor.write(170); 
            rightMotor.write(-170); 

            if (pidControl.leftOnTape() || pidControl.rightOnTape()) { 
                // nextState = ++state; 
                // state = 101; 
                // nextDelay = irPidTime; 
                // delay = millis() + 1000; 
                state++; 
                delay = millis() + 20;
            }
            break; 
        case 20: 
            // Temporary left jolt 
            rotateLeftJolt(); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + irPidTime;
            }
            break; 
        case 21: 
            // Temporary pid to align
            pid();
            if (millis() > delay) { 
                state = 2; 
                firstBridgeSequenceDone = true; 
            }
            break;
        // THROW SEQUENCE 
        case 22: 
            // Ewok grabbed, rotate for a set time
            leftClaw.stateOverride(CLAW_UP); 
            leftMotor.write(0); 
            rightMotor.write(255); 

            if (millis() > delay) { 
                nextState = ++state; 
                state = 101; 
                delay = millis() + 1000;
                nextDelay = 0;   
            }
            break; 
        case 23: 
            // Drive forward till edge detected
            leftMotor.write(230); 
            rightMotor.write(230); 

            if (bridge.detectLeftEdge() || bridge.detectRightEdge()) { 
                nextState = ++state; 
                nextDelay = 0; 
                state = 101; 
                delay = millis() + 2000; 
                leftClaw.stateOverride(CLAW_DOWN); 
            }
            break; 
        case 24: 
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + 450; 
                leftClaw.stateOverride(CLAW_UP); 
            }
            break; 
        case 25: 
            leftMotor.write(-255); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 250; 
            }
            break; 
        case 26: 
            // Rotate right, pivot on right wheel
            leftMotor.write(255); 
            rightMotor.write(0); 

            if (bridge.detectLeftEdge() && millis() > delay) { 
                state++; 
                delay = millis() + 50; 
            }
            break; 
        case 27: 
            // Temporary left jolt 
            rotateLeftJolt(); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + 450; 
            }
            break; 
        case 28: 
            // Drive forward until edge detected 
            leftMotor.write(defaultSpeed); 
            rightMotor.write(defaultSpeed-70); 

            if ((bridge.detectLeftEdge() || bridge.detectRightEdge()) && millis() > delay) { 
                state++; 
                delay = millis() + 75; 
            }
            break; 
        case 29: 
            // Temporary backward jolt 
            if (millis() < delay) { 
                leftMotor.write(-255); 
                rightMotor.write(-255); 
            }
            else { 
                state++; 
                delay = millis() + 3000; 
            }
            break; 
        case 30: 
            bridge.lowerBoth(); 
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) {  
                delay = millis() + 100; 
            }
            break; 
        case 31: 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed);

            if (millis() > delay) { 
                state++; 
                delay = millis() + 600; 
                bridge.raiseBoth();
                leftClaw.stateOverride(CLAW_DOWN); 
            }
            break; 
        case 32: 
            leftMotor.write(defaultSpeed-50); 
            rightMotor.write(defaultSpeed-50); 
            if (millis() > delay) { 
                state = 100; 
                leftClaw.stateOverride(CLAW_POLL); 
                rightClaw.stateOverride(CLAW_PHOENIX); 
            }
            break; 
        // THREE EWOK RETURN SEQUENCE
        case 39: 
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + 100; 
            }
            break; 
        case 40: 
            leftMotor.write(-255); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                nextState = ++state; 
                nextDelay = 50; 
                delay = millis() + 1350; 
                state = 101; 
            }
            break; 
        case 41: 
            // Quick rotation jolt to guard against stalling
            if (millis() < delay) { 
                rotateRightJolt();
            }
            else { 
                // Rotate right, pivot about center
                leftMotor.write(180);
                rightMotor.write(-180); 
            }  

            if (bridge.detectRightEdge()) { 
                state++; 
                delay = millis() + 50; 
            }
            break; 
        case 42: 
            // Temporary jolt to stop sliding after finding right edge
            if (millis() < delay) { 
                leftMotor.write(-255); 
                rightMotor.write(255); 
            }
            else { 
                leftMotor.write(0); 
                rightMotor.write(0); 
                leftClaw.stateOverride(CLAW_PHOENIX); 
                rightClaw.stateOverride(CLAW_PHOENIX);
            }

            if (!basket.readBasketSwitch()) { 
                basket.raiseBasket();
            }
            else { 
                state++; 
            }
            break; 
        case 43: 
            leftMotor.write(200);
            rightMotor.write(200);  
            basket.holdBasket(); 

            if (!basket.readBasketSwitch()) { 
                state++; 
                delay = millis() + 50; 
            }
            break; 
        case 44: 
            leftMotor.write(-255); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 1750; 
            }
            break; 
        case 45: 
            leftMotor.write(0); 
            rightMotor.write(0); 
            basket.lowerBasket(); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 1500; 
            }
            break; 
        case 46: 
            bridge.customAngle(80, 80); 
            leftMotor.write(-255); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                state = 100; 
            }
            break;
        // SECOND BRIDGE SEQUENCE
        case 50: 
            // Continue PIDing until edge detected
            pid(); 
            if (bridge.detectLeftEdge() && bridge.detectRightEdge()) { 
                state++; 
                delay = millis() + 110; 
            }
            break; 
        case 51:   
            // Quick backward jolt 
            leftMotor.write(-255); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 75; 
            }
            break; 
        case 52: 
            // Quick jolt to rotate right
            rotateRightJolt(); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 650; 
            }
            break; 
        case 53:   
            // Pivot about left wheel
            leftMotor.write(0); 
            rightMotor.write(-255); 

            if (millis() > delay) { 
                state++; 
            }
            break; 
        case 54: 
            // Short left jolt to stop rotation
            rotateLeftJolt(); 
            if (millis() > delay) { 
                // Finished 90 degree turn, temporary delay for a breather 
                nextState = ++state; 
                state = 101; 
                delay = millis() + 1000; 
                nextDelay = 0; 
            }
            break; 
        case 55: 
            // Finished 90 turn, roughly aligned, start forward drive until edge
            leftMotor.write(defaultSpeed); 
            rightMotor.write(defaultSpeed); 

            if (bridge.detectLeftEdge() && bridge.detectRightEdge()) { 
                state++; 
                delay = millis() + 50; 
            }
            else if (bridge.detectRightEdge() && !bridge.detectLeftEdge()) { 
                leftMotor.write(0); 
                rightMotor.write(-255); 
            }
            else if (bridge.detectLeftEdge() && !bridge.detectRightEdge()) { 
                leftMotor.write(-255); 
                rightMotor.write(0); 
            }
            break; 
        case 56: 
            // Quick backward jolt
            leftMotor.write(-255); 
            rightMotor.write(-255);  
            if (millis() > delay) { 
                state++; 
                delay = millis() + 250; 
            }
            break; 
        case 57: 
            // Back up slightly 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 50; 
            }
            break; 
        case 58:    
            // Temporary forward jolt 
            leftMotor.write(255); 
            rightMotor.write(255);

            if (millis() > delay) { 
                state++; 
                delay = millis() + 2000; 
                bridge.lowerBoth(); 
            } 
            break; 
        case 59: 
            // Lower bridge 
            bridge.lowerBoth(); 

            // Stop motors 
            leftMotor.write(0); 
            rightMotor.write(0); 

            if (millis() > delay) { 
                state++;
                delay = millis() + 300;  
            }
            break; 
        case 60: 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed); 

            if (millis() > delay) { 
                state++; 
                delay = millis() + 500; 
            }
            break; 
        case 61: 
            leftMotor.write(0); 
            rightMotor.write(0); 
            bridge.raiseBoth(); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + 500; 
            }
            break; 
        case 62:
            // Drive over bridge 
            leftMotor.write(defaultSpeed); 
            rightMotor.write(defaultSpeed); 

            if (millis() > delay) { 
                state = 100; 
            } 
            break; 
        case 63: 

            break; 
        case 100:
            leftMotor.write(0); 
            rightMotor.write(0);  
            basket.holdBasket(); 
            break; 
        case 101:
            // Wait for a set time and then advanced to a specifed state 
            leftMotor.write(0); 
            rightMotor.write(0); 

            if (millis() > delay) { 
                state = nextState; 
                delay = millis() + nextDelay; 
            }
            break; 
        default: 
            break; 
    }
}

void MotorControl::stateOverride(int specialState, int delay) { 
    /*  Manual state override for special motor sequences 
    *   Again, use with care, can get diffcult to debug if we flip flop states like my boy Willy Wonka
    *   Define unique motor states: 
    *   10 : First bridge deployment (reverse, wait, forward)
    *   20 : Infrared wait
    *   30 : Second bridge deployment (reverse, rotate 90 right, wait, forward) 
    *   40 : Edge detection algorithm
    *   50 : Basket sequence
    */
   state = specialState;
   this->delay = millis() + delay;  
}

/** 
 * Takes in an integer speed to be used as the new default speed. 
 * The default speed sets the base speed for PID control.
**/
void MotorControl::updateDefaultSpeed(int newSpeed) { 
    this->defaultSpeed = newSpeed; 
}

/**
 * Takes in an integer
 * Sets the current speeds being written to the motors.
**/
void MotorControl::updateSpeed(int newSpeed) { 
    this->speedLeft = newSpeed; 
    this->speedRight = newSpeed;
}

/** 
 * Takes in an integer 
 * Updates the speed of the left motor
**/ 
void MotorControl::updateSpeedLeft(int newSpeed) {
    speedLeft = newSpeed;
}

/** 
 * Takes in an integer
 * Updates the speed of the right motor
**/ 
void MotorControl::updateSpeedRight(int newSpeed) {
    speedRight = newSpeed;
}

/**
 * Takes in an integer
 * Updates the value of the QRD threshold (used to determine whether
 * on tape or not in PID control) 
**/ 
void MotorControl::updateThreshold(int newThreshold) { 
    qrdThreshold = newThreshold; 
}

void MotorControl::updateGain(int newGain) { 
    gain = newGain; 
}

void MotorControl::updateP(int newP) { 
    pVal = newP; 
}

void MotorControl::updateI(int newI) { 
    iVal = newI; 
}

void MotorControl::updateD(int newD) { 
    dVal = newD; 
}


void MotorControl::updateEdgeThreshold(int newThreshold) { 
    edgeThreshold = newThreshold; 
    bridge.updateThreshold(edgeThreshold); 
}

//// High level motor control functions //// 

void MotorControl::continuousForward() { 
    leftMotor.write(defaultSpeed); 
    rightMotor.write(defaultSpeed); 
}

void MotorControl::continuousReverse() { 
    leftMotor.write(-defaultSpeed); 
    rightMotor.write(-defaultSpeed); 
}   

void MotorControl::pid() { 
    // Bridge done, 10k signal, move with a reduced speed 
    if (firstBridgeSequenceDone && irGo && beforeStormTroopers) { 
        pidControl.followTape(qrdThreshold, gain-1, pVal-1, iVal, dVal, reducedSpeed);
    }
    else { 
        pidControl.followTape(qrdThreshold, gain, pVal, iVal, dVal, defaultSpeed);
    }
     
    updateSpeedLeft(pidControl.getLeftMotorSpeed()); 
    updateSpeedRight(pidControl.getRightMotorSpeed()); 
    
    leftMotor.write(speedLeft); 
    rightMotor.write(speedRight); 
}

void MotorControl::rotateLeft() { 
    leftMotor.write(-180); 
    rightMotor.write(180);
}

void MotorControl::rotateRight() { 
    leftMotor.write(215); 
    rightMotor.write(-215);
}

void MotorControl::rotateLeftJolt() { 
    leftMotor.write(-255); 
    leftMotor.write(255); 
}

void MotorControl::rotateRightJolt() { 
    rightMotor.write(255); 
    rightMotor.write(-255); 
}