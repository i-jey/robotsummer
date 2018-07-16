#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

///////// GLOBAL VARIABLE INITIALIZATION ///////// 
int temp = 0; 

///////// PINS ///////// 
// Menu pins
constexpr int btn = PB12; 
constexpr int incrementBtn = PB13; 
constexpr int pot = PB1; 
constexpr int startBtn = PB14; 

// Motor pins 
constexpr int left_motor_pin1 = PA2; 
constexpr int left_motor_pin2 = PA3; 
constexpr int right_motor_pin1 = PA0;
constexpr int right_motor_pin2 = PA1;  

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
constexpr int leftEdgeQRD = PA5; 
constexpr int rightEdgeQRD = PA6; 

///////// CONSTANTS BY MODULE /////////
// Claw 
int raiseClawDelay = 250; 
int openClawDelay = 1000; 
int lowerClawDelay = 500; 

// Bridge 
int bridge1Delay = 7500; 
int bridge2Delay = 7500; 
int rotateDelay = 2500; 
int edge_qrd_threshold = 400; 

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
int motorStartState = 0; 
int defaultSpeed = 150; 


int constants[] = {
    // Motor
    p, i, d, defaultSpeed, pid_qrd_threshold, motorStartState, 
    // Claw 
    raiseClawDelay, openClawDelay, lowerClawDelay, 
    // Bridge 
    reverseTime1, bridge1Delay, forwardDriveTime1, 
    reverseTime2, rotateDelay, bridge2Delay, forwardDriveTime2,
    // Basket

}; 
String constantNames[] = {
    // Motor
    "P-constant", "I-constant", "D-constant", "Default speed", "PID-QRD Threshold",  "Motor state",
    // Claw 
    "Raise delay", "Clamp open delay", "Lower arm delay", "Claw state", 
    //
};
///////// MODULES AND SEQUENCE DECLARATION /////////
// Motor control
Motor leftMotor;
Motor rightMotor;
MotorControl motorControl;

// Claws
Arm leftArm;
Arm rightArm;
ClawSequence leftClaw;
ClawSequence rightClaw;

// Bridge deployment
Bridge bridge; 
BridgeSequence bridgeSequence;

// IR Beacon TODO
// Basket sequence TODO

// Initialize display
OLED myOled(PB7, PB6, 8); 
extern uint8_t SmallFont[]; 

void setup() {
    Serial.begin(9600); 
    pinMode(right_push_button, INPUT); 
    pinMode(btn, INPUT_PULLUP); 
    pinMode(incrementBtn, INPUT_PULLUP);
    pinMode(pot, INPUT); 
    pinMode(startBtn, INPUT_PULLDOWN); 

    myOled.begin(); 
    myOled.setFont(SmallFont); 
}

bool editVal = false; 
void displayMenu() {
    int potVal = analogRead(pot); 
    int numConstants = sizeof(constants) / sizeof(constants[0]); 
    float incrementVal = 4096 / float(numConstants); 
    if (!digitalRead(btn)) {editVal = !editVal;}
    int btnVal = 0; 

    String optionName; 

    // Display raw analog value 
    myOled.printNumI(potVal, RIGHT, 0); 

    // Option #
    myOled.print("/", 5, 0); 
    myOled.printNumI(numConstants, 10, 0);

    // Display increment value
    myOled.print("Increment: ", 0, 10); 
    myOled.printNumI(incrementVal, RIGHT, 10);

    // Choose options
    for (int x = 1; x <= numConstants; x++) { 
        if (potVal < incrementVal*x) {
            optionName = constantNames[x-1]; 
            myOled.printNumI(x, 0, 0); 
            myOled.print(optionName, 0, 20); 
            myOled.printNumI(constants[x-1], RIGHT, 20); 
            myOled.print("View", RIGHT, 57); 
            
            while (editVal) { 
                delay(100); 
                if (!digitalRead(btn)){editVal=false;}
                if (!digitalRead(incrementBtn)){btnVal++;}
                potVal = analogRead(pot); 
                constants[x-1] = potVal + btnVal; 

                myOled.clrScr();
                myOled.print(optionName, 0, 20);  
                myOled.printNumI(constants[x-1], RIGHT, 20);  
                myOled.print("Edit", RIGHT, 57); 
                myOled.update(); 
            }
            return; 
        }
    }
}

bool start = true; 
bool firstRun = true; 
void loop() {
    if (!start) { 
        // Menu 
        start = digitalRead(startBtn); 
        myOled.clrScr(); 
        displayMenu(); 
        myOled.update(); 
        delay(100); 
    }
    else {  
        if (firstRun) { 
            firstRun = false; 
            ///////// MODULES AND SEQUENCE INITIALIZATION /////////
            // Motor control
            leftMotor = Motor(left_motor_pin1, left_motor_pin2);
            rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
            motorControl = MotorControl(motorStartState, defaultSpeed, leftMotor, rightMotor, 
                reverseTime1, reverseTime2, bridge1WaitTime, bridge2WaitTime, forwardDriveTime1, forwardDriveTime2); 

            // Claws
            leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button); 
            rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button);
            leftClaw = ClawSequence(leftArm, raiseClawDelay, openClawDelay, lowerClawDelay); 
            rightClaw = ClawSequence(rightArm, raiseClawDelay, openClawDelay, lowerClawDelay); 

            // Bridge deployment
            bridge = Bridge(bridge1_pin, bridge2_pin, leftEdgeQRD, rightEdgeQRD, edge_qrd_threshold);
            bridgeSequence = BridgeSequence(bridge, bridge1Delay, bridge2Delay, rotateDelay);

            // IR Beacon TODO
            // Basket sequence TODO

        }
        // Control loops
        else { 
            motorControl.poll(); 
            leftClaw.poll(); 
            rightClaw.poll(); 
        }
    }
}
