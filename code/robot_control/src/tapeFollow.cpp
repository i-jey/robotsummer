#include "includes.h"
#include "tapeFollow.h"

TapeFollow::TapeFollow(){}; 
TapeFollow::TapeFollow(int leftQRDPin, int rightQRDPin) { 
    this->leftQRDPin = leftQRDPin; 
    this->rightQRDPin = rightQRDPin; 
}

void TapeFollow::followTape(int qrdThreshold, int gain, int p, int i, int d, int defaultMotorSpeed) { 
    this->defaultMotorSpeed = defaultMotorSpeed;
    leftQRDState = analogRead(leftQRDPin) > qrdThreshold; 
    rightQRDState = analogRead(rightQRDPin) > qrdThreshold;

    // Serial.print("L: "); Serial.println(analogRead(leftQRDPin)); 
    // Serial.print("R: "); Serial.println(analogRead(rightQRDPin));
    // Serial.print("Threshold: "); Serial.println(qrdThreshold); 
    // Serial.print("LEFT: "); Serial.println(leftQRDState); 
    // Serial.print("RIGHT: "); Serial.println(rightQRDState); 
    
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
        this->leftMotorSpeed = defaultMotorSpeed + abs(pidGain); 
        this->rightMotorSpeed = defaultMotorSpeed - abs(pidGain); 
    } 
    else if (pidGain > 0) { 
        this->leftMotorSpeed = defaultMotorSpeed - abs(pidGain); 
        this->rightMotorSpeed = defaultMotorSpeed + abs(pidGain); 
    }
    else { 
        this->leftMotorSpeed = defaultMotorSpeed; 
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

int TapeFollow::getLeftQRDReading() { 
    return analogRead(leftQRDPin); 
}

int TapeFollow::getRightQRDReading() { 
    return analogRead(rightQRDPin); 
}

void TapeFollow::updateSpeed(int newSpeed) { 
    this->defaultMotorSpeed = newSpeed; 
}