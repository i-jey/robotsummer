#include "includes.h"

Bridge::Bridge(){}; // Default constructor otherwise C++ whines
Bridge::Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin, int qrdThreshold, int firstBridgeLowerAngle, int firstBridgeUpperAngle, int secondBridgeLowerAngle, int secondBridgeUpperAngle) { 
    this->bridgePin1 = bridgePin1; 
    this->bridgePin2 = bridgePin2; 
    this->QRDLeft = QRDLeftPin; 
    this->QRDRight = QRDRightPin;

    this->edgeThreshold = qrdThreshold;
    this->firstBridgeLowerAngle = firstBridgeLowerAngle; 
    this->firstBridgeUpperAngle = firstBridgeUpperAngle;
    this->secondBridgeLowerAngle = secondBridgeLowerAngle; 
    this->secondBridgeUpperAngle = secondBridgeUpperAngle; 
}

void Bridge::begin() { 
    bridgeServo1.attach(bridgePin1); 
    bridgeServo2.attach(bridgePin2); 
    pinMode(QRDLeft, INPUT_PULLDOWN); 
    pinMode(QRDRight, INPUT_PULLDOWN); 

    // Raise both bridges 
    bridgeServo1.write(firstBridgeUpperAngle); 
    bridgeServo2.write(secondBridgeUpperAngle); 
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
        bridgeServo1.write(firstBridgeLowerAngle); 
        bridgeServo2.write(secondBridgeLowerAngle); 
        return true; 
    }
    return false; 
}

void Bridge::raiseBoth() { 
    bridgeServo1.write(firstBridgeUpperAngle); 
    bridgeServo2.write(secondBridgeUpperAngle); 
}

void Bridge::customAngle(int angle1, int angle2) { 
    bridgeServo1.write(angle1);
    bridgeServo2.write(angle2);  
}

int Bridge::getLeftEdgeReading() { 
    return analogRead(QRDLeft); 
}

int Bridge::getRightEdgeReading() { 
    return analogRead(QRDRight); 
}

void Bridge::updateThreshold(int newThreshold) { 
    edgeThreshold = newThreshold; 
}

void Bridge::updateFirstBridgeLowerAngle(int newAngle) { 
    this->firstBridgeLowerAngle = newAngle; 
    lowerBridge1(newAngle); 
}

void Bridge::updateFirstBridgeUpperAngle(int newAngle) { 
    this->firstBridgeUpperAngle = newAngle; 
    raiseBridge1(); 
}