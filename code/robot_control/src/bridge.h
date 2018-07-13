#ifndef BRIDGE_H
#define BRIDGE_H

class Bridge { 
    private: 
        Servo bridgeServo1; 
        Servo bridgeServo2; 
        int QRDLeft; 
        int QRDRight; 

    public: 
        Bridge(); 
        Bridge(int bridgePin1, int bridgePin2, int QRDLeftPin, int QRDRightPin); 
        bool detectEdge(); 
        void lowerBridge1(int angle); 
        void raiseBridge1(); 
        bool lowerBoth(); 
        void raiseBoth(); 
};

#endif