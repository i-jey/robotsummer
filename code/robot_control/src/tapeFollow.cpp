#include "includes.h"
#include "tapeFollow.h"

TapeFollow::TapeFollow(){}; 
TapeFollow::TapeFollow(int leftQRDPin, int rightQRDPin) { 
    this->leftQRDPin = leftQRDPin; 
    this->rightQRDPin = rightQRDPin; 
}

void TapeFollow::followTape(int qrdThreshold, int gain, int p, int i, int d) { 
    leftQRDState = analogRead(leftQRDPin) > qrdThreshold; 
    rightQRDState = analogRead(rightQRDPin) > qrdThreshold;

    // Error cases 

    // Both QRDs on tape
    if (leftQRDState && rightQRDState) { 
        error = 0; 
    } 
    // Left off, right on
    else if (rightQRDState) { 
        error = -1; 
    }
    // Right off, left on
    else if (leftQRDState) { 
        error = 1; 
    }
    // Both off, right was previously on tape
    else if (lastError < 0)  { 
        error = -5; 
    }
    // Both off, left was previously on tape 
    else if (lastError > 0) { 
        error = 5; 
    }

    // Calculate PID gain 
    pidGain = (p*error) + (i*error + i) + (d*(error-lastError)); 
    pidGain *= gain; 

    if (pidGain < 0) { 
        this->leftMotorSpeed = defaultMotorSpeed + pidGain; 
        this->rightMotorSpeed = -defaultMotorSpeed; 
    } 
    if (pidGain > 0) { 
        this->leftMotorSpeed = -defaultMotorSpeed - pidGain; 
        this->rightMotorSpeed = defaultMotorSpeed; 
    }

    lastError = error; 
}

int TapeFollow::getLeftMotorSpeed() {
    return this->leftMotorSpeed; 
}

int TapeFollow::getRightMotorSpeed() { 
    return this->rightMotorSpeed; 
}