#include "includes.h"

// Left claw pins 
const int left_clamp_pin = PA1; 
const int left_arm_pin = PA2; 
const int left_push_button = PA3; 

// Right claw pins
const int right_clamp_pin = PA4; 
const int right_arm_pin = PA5;
const int right_push_button = PA6;  

// Bridge servos 
const int bridge1_pin = PA7; 
const int bridge2_pin = PA8; 

// Edge QRDs
const int leftEdgeQRD = PA11; 
const int rightEdgeQRD = PA12; 

const int raiseClawDelay = 250; 
const int openClawDelay = 1000; 
const int lowerClawDelay = 500; 

const int bridge1Delay = 7500; 
const int bridge2Delay = 7500; 
const int rotateDelay = 2500; 

Arm leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button); 
Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button);
ClawSequence leftClaw = ClawSequence(leftArm, raiseClawDelay, openClawDelay, lowerClawDelay); 
ClawSequence rightClaw = ClawSequence(rightArm, raiseClawDelay, openClawDelay, lowerClawDelay); 

Bridge bridge = Bridge(bridge1_pin, bridge2_pin, leftEdgeQRD, rightEdgeQRD);
BridgeSequence bridgeSequence = BridgeSequence(bridge, bridge1Delay, bridge2Delay, rotateDelay);

unsigned long prevTime = 0; 

void setup() {
    Serial.begin(9600); 
}

void loop() {
    leftClaw.poll(); 
    rightClaw.poll(); 
    bridgeSequence.poll(); 
}
