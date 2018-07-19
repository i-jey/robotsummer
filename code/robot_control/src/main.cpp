#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

// Oled pins 
constexpr int oled_scl = PB10; 
constexpr int oled_sda = PB11; 
OLED myOled(oled_scl, oled_sda, 8); 
extern uint8_t SmallFont[];

// Motor pins 
constexpr int right_motor_pin1 = PA0; 
constexpr int right_motor_pin2 = PA1; 
constexpr int left_motor_pin1 = PA2; 
constexpr int left_motor_pin2 = PA3; 

// PID QRDs 
constexpr int right_pid_QRD = PA4; 
constexpr int left_pid_QRD = PA5; 

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
int qrdThreshold = 50; 
int gain = 5;
int p = 1; 
int i = 0; 
int d = 4; 
int pid_qrd_threshold = 200; 

// Start state and speed
int motorStartState = 0; 
int defaultSpeed = 50; 
TapeFollow pidControl = TapeFollow(left_pid_QRD, right_pid_QRD); 

// Initialize motor
Motor rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
Motor leftMotor = Motor(left_motor_pin1, left_motor_pin2); 
MotorControl motorControl = MotorControl(motorStartState, defaultSpeed, leftMotor, rightMotor, 
    pidControl, qrdThreshold, gain, p, i, d, reverseTime1, reverseTime2, bridge1WaitTime, 
    bridge2WaitTime, forwardDriveTime1, forwardDriveTime2); 

// Claw 
int closeTime = 250; 
int raiseTime = 1000; 
int openTime = 500; 
int closeTime2 = 500; 
int lowerTime = 1000; 
int resetTime = 500; 

// Initialize arms and claws
Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button); 
ClawSequence rightClaw = ClawSequence(rightArm, closeTime, raiseTime, openTime, closeTime2, lowerTime, resetTime); 
Arm leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button); 
ClawSequence leftClaw = ClawSequence(leftArm, closeTime, raiseTime, openTime, closeTime2, lowerTime, resetTime); 

// Menu pins 
bool start; 
bool toggle = false; 
constexpr int startBtn = PC15;
constexpr int menuPot = PB1; 
constexpr int menuPlus = PA12; 
constexpr int menuMinus = PA15; 

void setup() { 
    Serial.begin(9600); 
    
    // Initialize motor pins 
    MotorInit motorInit = MotorInit(); 
    motorInit.init(); 
    
    // Initialize claw buttons
    pinMode(right_push_button, INPUT_PULLDOWN); 
    pinMode(left_push_button, INPUT_PULLDOWN); 

    // Menu dials 
    pinMode(oled_scl, INPUT_PULLUP); 
    pinMode(oled_sda, INPUT_PULLUP); 
    pinMode(startBtn, INPUT_PULLDOWN);
    start = digitalRead(startBtn);  
    pinMode(menuPot, INPUT); 
    pinMode(menuPlus, INPUT_PULLDOWN); 
    pinMode(menuMinus, INPUT_PULLDOWN); 
    myOled.begin(); 
    myOled.setFont(SmallFont); 

}

int optionState = 0; 
void pidMenu() { 
    int potVal = analogRead(menuPot) / 10; 
    
    myOled.print("PID Menu", 0, 0); 
    myOled.print("p: ", 0, 10); 
    myOled.print("d: ", 0, 20); 
    myOled.print("gain: ", 0, 30); 
    myOled.print("thresh: ", 0, 40); 

    delay(100); 
    if (!digitalRead(menuPlus)) {
        if (optionState == 3) 
            {optionState = 0;
        }
        else {
            optionState++;
        }
    }
    if (digitalRead(menuMinus)) {toggle = !toggle;}

    myOled.printNumI(p, RIGHT, 10);
    myOled.printNumI(d, RIGHT, 20);
    myOled.printNumI(gain, RIGHT, 30);
    myOled.printNumI(qrdThreshold, RIGHT, 40);
    myOled.print("Mode: ", 0, 50); 
    myOled.print("<--", 45, (optionState+1)*10); 

    if (toggle) { 
        myOled.print("Edit", RIGHT, 50); 
    
        switch (optionState) { 
            case 0:
                p = potVal; 
                motorControl.updateP(potVal);  
                break; 
            case 1: 
                d = potVal; 
                motorControl.updateD(potVal); 
                break; 
            case 2: 
                gain = potVal; 
                motorControl.updateGain(potVal);
                break; 
            case 3: 
                qrdThreshold = potVal; 
                motorControl.updateThreshold(potVal);
                break; 
            default:
                break; 
        }
    }
    else {
         myOled.print("View", RIGHT, 50); 
    }
}

void loop() { 
    start = digitalRead(startBtn); 
    Serial.println(start); 
    if (!start) { 
        myOled.clrScr(); 
        pidMenu(); 
        myOled.update();  
    }
    else {
        motorControl.poll(); 
        rightClaw.poll(); 
        leftClaw.poll(); 
    }
}