#include "includes.h"
#include "bridge.h"

constexpr int OFF_EDGE_THRESHOLD = 400; 

Bridge::Bridge(){}; // Default constructor otherwise C++ whines
Bridge::Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin) { 
    bridgeServo1.attach(bridgePin1); 
    bridgeServo2.attach(bridgePin2); 
    this->QRDLeft = QRDLeftPin; 
    this->QRDRight = QRDRightPin;
}

bool Bridge::detectEdge() { 
    int leftReading = analogRead(QRDLeft); 
    int rightReading = analogRead(QRDRight); 
    Serial.print("Left: ");
    Serial.println(leftReading);  
    Serial.print("Right: ");
    Serial.println(rightReading); 

    if (analogRead(QRDLeft) + analogRead(QRDRight) < 2*OFF_EDGE_THRESHOLD) { 
        return true; 
    }
    return false; 
}
void Bridge::lowerBridge1(int angle) { 
    bridgeServo1.write(angle); 
    // if (detectEdge()) { 
    //     // Need to test incrementally lowering bridge 
    //     // Can use for loop with a delay(), but is there a
    //     // better non-blocking way?
    //     bridgeServo1.write(47); 
    //     return true; 
    // }
}

void Bridge::raiseBridge1() { 
    bridgeServo1.write(140); 
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