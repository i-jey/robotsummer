#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

// Right claw pins 
constexpr int right_clamp_pin = PB6; 
constexpr int right_arm_pin = PB7; 
constexpr int right_push_button = PB4; 

// Claw 
int raiseClawDelay = 250; 
int openClawDelay = 1000; 
int lowerClawDelay = 1000; 

Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button); 
ClawSequence rightClaw = ClawSequence(rightArm, raiseClawDelay, openClawDelay, lowerClawDelay); 

void setup() { 
    Serial.begin(9600); 
    
    // Right claw pinModes 
    pinMode(right_push_button, INPUT_PULLDOWN); 
}

void loop() { 
    rightClaw.poll(); 
}