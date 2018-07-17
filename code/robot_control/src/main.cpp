#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

///////// GLOBAL VARIABLE INITIALIZATION ///////// 
int temp = 0; 

///////// PINS ///////// 
// Menu pins
constexpr int btn = PB12; 
constexpr int incrementBtn = PB13; 
constexpr int pot = PB0;
constexpr int pot2 = PB1;  
constexpr int startBtn = PB14; 

// Motor pins 
constexpr int left_motor_pin1 = PA2; 
constexpr int left_motor_pin2 = PA3; 
constexpr int right_motor_pin1 = PA0;
constexpr int right_motor_pin2 = PA1;  

// Left claw pins 
constexpr int left_clamp_pin = PA8; 
constexpr int left_arm_pin = PA9; 
constexpr int left_push_button = PB15; 

// Right claw pins
constexpr int right_clamp_pin = PB6; 
constexpr int right_arm_pin = PB7;
constexpr int right_push_button = PB4;  

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
int defaultSpeed = 50; 


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

// Encoder 
int e1 = PB5; 
int e2 = PB6; 
int e3 = PB8; 
int e4 = PB9; 
Encoder encoder = Encoder(e1, e2, e3, e4); 
// Initialize display
OLED myOled(PB7, PB6, 8); 
extern uint8_t SmallFont[]; 

void setup() {
    Serial.begin(9600); 
    pinMode(right_push_button, INPUT_PULLDOWN);
    pinMode(left_push_button, INPUT_PULLDOWN);  
    pinMode(btn, INPUT_PULLUP); 
    pinMode(incrementBtn, INPUT_PULLUP);
    pinMode(pot, INPUT); 
    pinMode(pot2, INPUT); 
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
    myOled.print("/", 7, 0); 
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

void displayPID() { 
    int p = analogRead(pot); 
    int d = analogRead(pot2); 

    myOled.print("PID Menu", 0, 0); 
    myOled.print("P-constant: ", 0, 10); 
    myOled.printNumI(p, RIGHT, 10); 
    myOled.print("D-constant", 0, 20); 
    myOled.printNumI(d, RIGHT, 20); 
}

bool start = false; 
bool pidMenu = true; 
bool firstRun = true; 
void loop() {
    if (false) { 
        start = digitalRead(startBtn); 
        
        if (pidMenu) { 
            myOled.clrScr(); 
            displayPID(); 
            myOled.update(); 
        }
        else {
            // Display regular menu 
            myOled.clrScr(); 
            displayMenu(); 
            myOled.update(); 
        }

        delay(100); 
    }
    else {  
        if (firstRun) { 
            firstRun = false; 
            ///////// MODULES AND SEQUENCE INITIALIZATION /////////
            // Motor control
            leftMotor = Motor(left_motor_pin1, left_motor_pin2);
            rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
            motorControl = MotorControl(motorStartState, defaultSpeed, leftMotor, rightMotor, p, i, d,
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
            // Serial.println(encoderValR); 
            // Serial.println(encoderValL);  
            // myOled.clrScr(); 
            // myOled.print("Right speed: ", 0, 0); 
            // myOled.print("Left speed:", 0, 10); 
            // myOled.printNumI(encoderValR, RIGHT, 0); 
            // myOled.printNumI(encoderValL, RIGHT, 10); 
            // myOled.update(); 
            // motorControl.poll(); 
            // leftClaw.poll(); 
            Serial.println(digitalRead(right_push_button)); 
            rightClaw.poll(); 
        }
    }
}
