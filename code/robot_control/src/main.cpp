#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

// Motor pins 
constexpr int right_motor_pin1 = PA0; 
constexpr int right_motor_pin2 = PA1; 
constexpr int left_motor_pin1 = PA2; 
constexpr int left_motor_pin2 = PA3; 

// Right claw pins 
constexpr int right_clamp_pin = PB8; 
constexpr int right_arm_pin = PB7; 
constexpr int right_push_button = PB4; 

// Left claw pins 
constexpr int left_clamp_pin = PB6;
constexpr int left_arm_pin = PB5; 
constexpr int left_push_button = PB3; 

// Motor 
// How long to run motor in reverse when edge detected 
int reverseTime1 = 1500; 
int reverseTime2 = 1500; 
// Motor wait times while lowering bridge 
int bridge1WaitTime = 7500; 
int bridge2WaitTime = 7500; 
// How long to drive forward after dropping bridge 
int forwardDriveTime1 = 2000; 
int forwardDriveTime2 = 2000; 
// PID constants
int p = 50; 
int i = 0; 
int d = 37; 
int pid_qrd_threshold = 200; 

// Start state and speed
int motorStartState = 0; 
int defaultSpeed = 60; 

Motor rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
Motor leftMotor = Motor(left_motor_pin1, left_motor_pin2); 
MotorControl motorControl = MotorControl(motorStartState, defaultSpeed, leftMotor, rightMotor, 
    p, i, d, reverseTime1, reverseTime2, bridge1WaitTime, 
    bridge2WaitTime, forwardDriveTime1, forwardDriveTime2); 

// Claw 
int raiseClawDelay = 250; 
int openClawDelay = 1000; 
int lowerClawDelay = 1000; 

Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button); 
ClawSequence rightClaw = ClawSequence(rightArm, raiseClawDelay, openClawDelay, lowerClawDelay); 

Arm leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button); 
ClawSequence leftClaw = ClawSequence(leftArm, raiseClawDelay, openClawDelay, lowerClawDelay); 

void setup() { 
    Serial.begin(9600); 
    
    // Initialize motor pins 
    MotorInit motorInit = MotorInit(); 
    motorInit.init(); 
    
    // Right claw pinModes 
    pinMode(right_push_button, INPUT_PULLDOWN); 
    pinMode(left_push_button, INPUT_PULLDOWN); 
}

void loop() { 
    motorControl.poll(); 
    rightClaw.poll(); 
    leftClaw.poll(); 
}