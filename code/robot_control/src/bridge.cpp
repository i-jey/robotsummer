#include "includes.h"
#include "bridge.h"

Bridge::Bridge(){}; // Default constructor otherwise C++ whines
Bridge::Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin, int qrdThreshold, int firstBridgeLowerAngle, int firstBridgeUpperAngle) { 
    bridgeServo1.attach(bridgePin1); 
    bridgeServo2.attach(bridgePin2); 
    this->QRDLeft = QRDLeftPin; 
    this->QRDRight = QRDRightPin;
    this->qrdThreshold = qrdThreshold; 
    this->firstBridgeLowerAngle = firstBridgeLowerAngle; 
    this->firstBridgeUpperAngle = firstBridgeUpperAngle;
    bridgeServo1.write(firstBridgeUpperAngle);
}

bool Bridge::detectEdge() { 
    int leftReading = getLeftEdgeReading(); 
    int rightReading = getRightEdgeReading(); 

    // Serial.println(leftReading); 
    // Serial.println(rightReading); 
    if (leftReading > qrdThreshold && rightReading > qrdThreshold) { 
        return true; 
    }
    return false; 
}
void Bridge::lowerBridge1(int angle) { 
    bridgeServo1.write(angle); 
}

void Bridge::raiseBridge1() { 
    bridgeServo1.write(firstBridgeUpperAngle); 
}

bool Bridge::lowerBoth() { 
    if (detectEdge()) { 
        bridgeServo1.write(47); 
        bridgeServo2.write(47); 
        return true; 
    }
    return false; 
}

void Bridge::raiseBoth() { 
    bridgeServo1.write(140); 
    bridgeServo2.write(140); 
}

int Bridge::getLeftEdgeReading() { 
    return analogRead(QRDLeft); 
}

int Bridge::getRightEdgeReading() { 
    return analogRead(QRDRight); 
}

void Bridge::updateThreshold(int newThreshold) { 
    qrdThreshold = newThreshold; 
}

void Bridge::updateFirstBridgeLowerAngle(int newAngle) { 
    this->firstBridgeLowerAngle = newAngle; 
    lowerBridge1(newAngle); 
}

void Bridge::updateFirstBridgeUpperAngle(int newAngle) { 
    this->firstBridgeUpperAngle = newAngle; 
    raiseBridge1(); 
}