#include "includes.h"

// Motor pins 
constexpr int left_motor_pin1 = PA0; 
constexpr int left_motor_pin2 = PA1; 
constexpr int right_motor_pin1 = PA2;
constexpr int right_motor_pin2 = PA3;  

// Left claw pins 
constexpr int left_clamp_pin = PA1; 
constexpr int left_arm_pin = PA2; 
constexpr int left_push_button = PA3; 

// Right claw pins
constexpr int right_clamp_pin = PB0; 
constexpr int right_arm_pin = PB1;
constexpr int right_push_button = PB6;  

// Bridge servos 
constexpr int bridge1_pin = PA7; 
constexpr int bridge2_pin = PA8; 

// Edge QRDs
constexpr int leftEdgeQRD = PA11; 
constexpr int rightEdgeQRD = PA12; 

// Claw delay times
constexpr int raiseClawDelay = 250; 
constexpr int openClawDelay = 1000; 
constexpr int lowerClawDelay = 500; 

// Bridge lowering delaying times
constexpr int bridge1Delay = 7500; 
constexpr int bridge2Delay = 7500; 
constexpr int rotateDelay = 2500; 

// Motor wait times while lowering bridge 
constexpr int bridge1WaitTime = 2500; 
constexpr int bridge2WaitTime = 2500; 

// How long to run motor in reverse when edge detected 
constexpr int reverseTime1 = 500; 
constexpr int reverseTime2 = 500; 

// Motor control
Motor leftMotor = Motor(left_motor_pin1, left_motor_pin2);
Motor rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
MotorControl motorControl = MotorControl(leftMotor, rightMotor, bridge1WaitTime, bridge2WaitTime, reverseTime1, reverseTime2); 

// Claws
Arm leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button); 
Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button);
ClawSequence leftClaw = ClawSequence(leftArm, raiseClawDelay, openClawDelay, lowerClawDelay); 
ClawSequence rightClaw = ClawSequence(rightArm, raiseClawDelay, openClawDelay, lowerClawDelay); 

// Bridge deployment
Bridge bridge = Bridge(bridge1_pin, bridge2_pin, leftEdgeQRD, rightEdgeQRD);
BridgeSequence bridgeSequence = BridgeSequence(bridge, bridge1Delay, bridge2Delay, rotateDelay);

// IR Beacon TODO
// Basket sequence TODO

void setup() {
    Serial.begin(9600); 
    pinMode(right_push_button, INPUT); 
}

void loop() {
    leftClaw.poll(); 
    rightClaw.poll(); 
    bridgeSequence.poll(); 
}
