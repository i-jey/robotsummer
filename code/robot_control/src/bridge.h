#ifndef BRIDGE_H
#define BRIDGE_H

class Bridge { 
    private: 
        Servo bridgeServo1; 
        Servo bridgeServo2; 
        uint8_t QRDLeft; 
        uint8_t QRDRight; 
        uint8_t QRD_THRESHOLD; 

    public: 
        Bridge(); 
        Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin, 
                int QRD_THRESHOLD, int firstBridgeLowerAngle, int firstBridgeUpperAngle); 
        bool detectEdge(); 
        void lowerBridge1(int angle); 
        void raiseBridge1(); 
        bool lowerBoth(); 
        void raiseBoth(); 
        int getLeftEdgeReading(); 
        int getRightEdgeReading(); 
        void updateThreshold(int newThreshold); 
        void updateFirstBridgeLowerAngle(int newAngle); 
        void updateFirstBridgeUpperAngle(int newAngle); 
        uint8_t firstBridgeUpperAngle; 
        uint8_t firstBridgeLowerAngle; 
        uint8_t secondBridgeLowerAngle; 
        uint8_t secondBridgeUpperAngle; 
};

#endif