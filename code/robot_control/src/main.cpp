#include "includes.h"

// Left claw pins 
int left_clamp_pin = PA1; 
int left_arm_pin = PA2; 
int left_push_button = PA3; 

// Right claw pins
int right_clamp_pin = PA4; 
int right_arm_pin = PA5;
int right_push_button = PA6;  

// Bridge servos 
int bridge1_pin = PA7; 
int bridge2_pin = PA8; 

// Edge QRDs
int leftEdgeQRD = PA11; 
int rightEdgeQRD = PA12; 

int raiseClawDelay = 250; 
int openClawDelay = 1000; 
int lowerClawDelay = 500; 

int bridge1Delay = 7500; 
int bridge2Delay = 7500; 
int rotateDelay = 2500; 

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
