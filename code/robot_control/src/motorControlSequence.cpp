#include "includes.h" 

// flags 
bool firstBridgeSequenceDone = false; 
bool throughGate = false; 
bool afterStormtroopers = true; 
bool detected1k = false; 
bool irGo = false; 
int reducedSpeed;

MotorControl::MotorControl(){};
MotorControl::MotorControl(Motor &leftMotor, Motor &rightMotor, Bridge &bridge, IRReader &ir, 
    Basket &basket, TapeFollow &pidControl, ClawSequence &leftClaw, ClawSequence &rightClaw, int qrdThreshold, int gain, int p, int i, int d) { 

    this->leftMotor = leftMotor; 
    this->rightMotor = rightMotor; 
    this->bridge = bridge; 
    this->ir = ir; 
    this->basket = basket; 
    this->pidControl = pidControl; 
    this->leftClaw = leftClaw; 
    this->rightClaw = rightClaw; 
    this->qrdThreshold = qrdThreshold;  
    this->gain = gain; 
    this->pVal = p; 
    this->iVal = i; 
    this->dVal = d; 

    // Initialize bridge angle 
    bridge.raiseBridge1(); 
    bridge.firstBridgeLowerAngle = 40;
    bridge.firstBridgeUpperAngle = 130; 

    reducedSpeed = 130; 
}

void MotorControl::reset() { 
    firstBridgeSequenceDone = false; 
    throughGate = false; 
    afterStormtroopers = true; 
    detected1k = false; 
    irGo = false; 
    reducedSpeed = defaultSpeed - 45;
}

void MotorControl::specialStateChecker() {
    // Special state delay is used to prevent repeating a state (i.e restarting first bridge sequence multiple times as
    // it pulls away from the edge)
    
    if (millis() < specialStateDelay) { 
        return; 
    }

    // Bridge special sequences 
    if (bridge.detectLeftEdge()) { 
        specialStateDelay = millis() + 50; 

        if (edgeCounters == 0 && ewokCounter == 1) { 
            // Switch to first bridge sequence
            state = 10; 
            edgeCounters++; 
            globalClawStateTracker = 2; // Lift left arm
        }

        if (edgeCounters == 1 && ewokCounter == 3) { 
            // Switch to second edge sequence
            state = 20; 
        }
    }

    // IR special sequences  
    if (ir.read1k() || ir.read10k()) { 
        if (detected1k == false && firstBridgeSequenceDone == true && ewokCounter == 2) { 
            // Switch to wait state
            state = 100;  
            if (ir.read1k()) {detected1k = true;}
            globalClawStateTracker = 2; // Lift left arm 
        }
        else if (ir.read10k() && !ir.read1k() && irGo == false && firstBridgeSequenceDone == true) { 
            // Switch to haul ass
            state = 2; 
            irGo = true; 
            defaultSpeed = reducedSpeed; // Slow down once 10k is detected to go through gate and pick up ewok
        }
    }
    if (ewokCounter == 2 && !throughGate) { 
        // Freeze, both hands up! oh god please don't shoot please im too yung
        globalClawStateTracker = 3; 
        throughGate = true; 
        specialStateDelay = millis() + 3000; // 4 second delay to get from gate to after storm troopers before bring left claw down
    }
    if (millis() > specialStateDelay && ewokCounter == 2 && afterStormtroopers) { 
        // Left claw down, keep right up 
        globalClawStateTracker = 4; 
        afterStormtroopers = false; 
    }
}

float angleToCounts(int angle) { 
    /**
     *      Angle --> encoder counts
     *      
     *      num counts      angle 
     *      ----------  *   -----
     *         2pir          360
    **/
    float circumference = 3.14159265 * wheelDiameter; 
    float arclength = (angle / 360.0) * 3.14159265*wheelToWheelDistance; 
    float encoderCounts = (countsPerRotation / circumference) * arclength;
    return encoderCounts; 
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
                state = 20; 
            }
            break; 
        case 4: 
            rotateRight();

            if (millis() > delay) { 
                state = 20; 
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
        case 7: 
            // Rotate until tape is found        
            rotateLeft(); 

            if (millis() > delay) { 
                delay = millis();
                leftMotor.write(defaultSpeed+40); 
                rightMotor.write(defaultSpeed-40);
                state = 99; 
            }
            
            break; 
        // FIRST BRIDGE SEQUENCE
        case 10: 
            // Edge detected on left, align with right
            leftMotor.write(-180); 
            rightMotor.write(0); 

            if (bridge.detectRightEdge()) { 
                state++; 
                leftWheelCounter = 0; 
                rightWheelCounter = 0;
                delay = millis() + 1000;  
            } 
            break; 
        case 11: 
            // Temporary stop 
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) { 
                state++; 
                delay = millis() + edgeReverseDistance; 
            }
            break;
        case 12: 
            // Reverse 
            leftMotor.write(-defaultSpeed); 
            rightMotor.write(-defaultSpeed); 

            // if (leftWheelCounter > edgeReverseDistance && rightWheelCounter > edgeReverseDistance) { 
            //     state++; 
            //     leftWheelCounter = 0; 
            //     rightWheelCounter = 0; 
            // }
            if (millis() > delay) { 
                state++;  
                angle = bridge.firstBridgeUpperAngle;
            }
            break; 
        case 13: 
            // Stop motors 
            leftMotor.write(0); 
            rightMotor.write(0); 

            if (millis() > delay) { 
                // Lower bridge 
                bridge.lowerBridge1(angle); 
                angle--;
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
            // if (leftWheelCounter > dropBridgeDistance && rightWheelCounter > dropBridgeDistance) { 
            //     state++; 
            //     leftWheelCounter = 0; 
            //     rightWheelCounter = 0; 
            //     delay = millis() + 1000; 
            // }
            if (millis() > delay) { 
                state++; 
                delay = millis() + 1000; 
            }
            break; 
        case 15: 
            // Stop motors 
            leftMotor.write(0); 
            rightMotor.write(0); 
            if (millis() > delay) { 
                state++; 
                bridge.raiseBridge1(); 
                delay = millis() + driveOverDistance;
            }
            break; 
        case 16: 
            // Drive over bridge
            leftMotor.write(150 + 15); 
            rightMotor.write(150 - 15); 
            // if (leftWheelCounter > driveOverDistance && rightWheelCounter > driveOverDistance) { 
            //     // start PID again 
            //     state = 100; 
            //     leftWheelCounter = 0; 
            //     rightWheelCounter = 0; 
            // } 

            if (millis() > delay && (pidControl.leftOnTape() || pidControl.rightOnTape())) { 
                state = 2; 
                firstBridgeSequenceDone = true; 
            }
        
            // if (pidControl.leftOnTape() || pidControl.rightOnTape() ) { 
            //     state = 2; 
            // }

            break; 
        case 30: 
            // TODO
            // Custom bridge dropping maneuver
            break; 
        case 40: 
            // TODO 
            // Edge detection
            // damn this will be a doozy
            break; 
        case 50: 
            // Once last ewok has been picked up, basket drop 
            // Reverse motors and raise basket in parallel, wait, drive forward
            break; 
        case 99: 
            // Hold whatever motor speeds are currently going
            break; 
        case 100: 
            // Stop motors until delay is up
            leftMotor.write(0); 
            rightMotor.write(0);  
            // globalClawStateTracker = 1; 
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
    pidControl.followTape(qrdThreshold, gain, pVal, iVal, dVal, defaultSpeed); 

    updateSpeedLeft(pidControl.getLeftMotorSpeed()); 
    updateSpeedRight(pidControl.getRightMotorSpeed()); 
    
    leftMotor.write(speedLeft); 
    rightMotor.write(speedRight); 
}

void MotorControl::rotateLeft() { 
    leftMotor.write(-200); 
    rightMotor.write(200);
}

void MotorControl::rotateRight() { 
    leftMotor.write(200); 
    rightMotor.write(-200);
}
