#include "includes.h"

Bridge::Bridge(){}; // Default constructor otherwise C++ whines
Bridge::Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin, int qrdThreshold, int firstBridgeLowerAngle, int firstBridgeUpperAngle) { 
    bridgeServo1.attach(bridgePin1); 
    bridgeServo2.attach(bridgePin2); 
    this->QRDLeft = QRDLeftPin; 
    this->QRDRight = QRDRightPin;
    this->edgeThreshold = qrdThreshold;
    this->firstBridgeLowerAngle = firstBridgeLowerAngle; 
    this->firstBridgeUpperAngle = firstBridgeUpperAngle;
    bridgeServo1.write(firstBridgeUpperAngle);
}

bool Bridge::detectLeftEdge() { 
    int leftReading = getLeftEdgeReading(); 

    if (leftReading > edgeThreshold) { 
        return true; 
    }
    return false; 
}


bool Bridge::detectRightEdge() { 
    int rightReading = getRightEdgeReading(); 
    Serial.println(edgeThreshold); 
    if (rightReading > edgeThreshold) { 
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
    if (detectLeftEdge() && detectRightEdge()) { 
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
    edgeThreshold = newThreshold; 
    Serial.println(edgeThreshold);  
}

void Bridge::updateFirstBridgeLowerAngle(int newAngle) { 
    this->firstBridgeLowerAngle = newAngle; 
    lowerBridge1(newAngle); 
}

void Bridge::updateFirstBridgeUpperAngle(int newAngle) { 
    this->firstBridgeUpperAngle = newAngle; 
    raiseBridge1(); 
}