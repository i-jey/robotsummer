#ifndef BRIDGE_H
#define BRIDGE_H

class Bridge { 
    private: 
        Servo bridgeServo1; 
        Servo bridgeServo2; 
        int bridgePin1; 
        int bridgePin2; 
        int QRDLeft; 
        int QRDRight; 
        int edgeThreshold;

    public: 
        Bridge(); 
        Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin, int qrdThreshold, 
            int firstBridgeLowerAngle, int firstBridgeUpperAngle, int secondBridgeLowerAngle, int secondBridgeUpperAngle); 
        void begin();
        bool detectLeftEdge(); 
        bool detectRightEdge(); 
        void lowerBridge1(int angle); 
        void raiseBridge1(); 
        bool lowerBoth(); 
        void raiseBoth(); 
        void customAngle(int angle1, int angle2); 
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