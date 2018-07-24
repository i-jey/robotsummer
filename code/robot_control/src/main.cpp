#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

// EEPROM address  
// uint16 startingAddress = 0x10; 

// EEPROM / menu reference 
enum MenuItems { 
    menu_p,
    menu_d, 
    menu_gain, 
    menu_qrdThreshold, 
    menu_defaultSpeed
};

// Oled pins 
constexpr int oled_scl = PB10; 
constexpr int oled_sda = PB11; 
OLED oled(oled_scl, oled_sda, 8); 
extern uint8_t SmallFont[];

// Motor pins 
constexpr int right_motor_pin1 = PA0; 
constexpr int right_motor_pin2 = PA1; 
constexpr int left_motor_pin1 = PA2; 
constexpr int left_motor_pin2 = PA3; 

// PID QRDs 
constexpr int right_pid_QRD = PA4; 
constexpr int left_pid_QRD = PA7; 

// Edge QRDs 
constexpr int right_edge_QRD = PA5; 
constexpr int left_edge_QRD = PA6; 

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
int p = 0;
int i = 0; 
int d = 0;
int gain = 0;
int qrdThreshold = 0; 

// Start state and speed
int motorStartState = 2; 
int defaultSpeed; 
TapeFollow pidControl = TapeFollow(left_pid_QRD, right_pid_QRD); 

// Initialize motor
Motor rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
Motor leftMotor = Motor(left_motor_pin1, left_motor_pin2); 
MotorControl motorControl = MotorControl(motorStartState, defaultSpeed, leftMotor, rightMotor, 
    pidControl, qrdThreshold, gain, p, i, d, reverseTime1, reverseTime2, bridge1WaitTime, 
    bridge2WaitTime, forwardDriveTime1, forwardDriveTime2); 

// Claw 
int leftClawCloseAngle = 140; 
int leftClawOpenAngle = 50; 
int leftClawOpenAngleInside; 
int leftClawLowerAngle = 0; 
int leftClawRaiseAngle = 180; 

int rightClawCloseAngle = 0; 
int rightClawOpenAngle = 110; 
int rightClawOpenAngleInside = 87; 
int rightClawLowerAngle = 142; 
int rightClawRaiseAngle = 10; 
int rightVertical = 40; 

int closeTime = 250; 
int raiseTime = 1000; 
int openTime = 500; 
int closeTime2 = 500; 
int lowerTime = 1000; 
int resetTime = 500; 

// Initialize arms and claws 
Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button, rightClawCloseAngle, rightClawOpenAngle, rightClawOpenAngleInside, rightClawLowerAngle, rightClawRaiseAngle); 
ClawSequence rightClaw = ClawSequence(rightArm, closeTime, raiseTime, openTime, closeTime2, lowerTime, resetTime); 
Arm leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button, leftClawCloseAngle, leftClawOpenAngle, leftClawOpenAngleInside, leftClawLowerAngle, leftClawRaiseAngle); 
ClawSequence leftClaw = ClawSequence(leftArm, closeTime, raiseTime, openTime, closeTime2, lowerTime, resetTime); 

// Menu pins 
bool start; 
bool toggle = false; 
constexpr int menuToggle = PA11; 
constexpr int menuPlus = PA12; 
constexpr int menuMinus = PA15; 
constexpr int menuPot = PB1; 
constexpr int startBtn = PC15;

void setup() {  
    Serial.begin(9600); 
    
    // Initialize motor pins 
    MotorInit motorInit = MotorInit(); 
    motorInit.init(); 
    
    // Initialize claw buttons
    pinMode(right_push_button, INPUT_PULLDOWN); 
    pinMode(left_push_button, INPUT_PULLDOWN); 
    rightArm.open(false); 
    leftArm.open(false); 
    rightArm.lower(); 
    leftArm.lower(); 

    // Menu dials 
    pinMode(oled_scl, INPUT_PULLUP); 
    pinMode(oled_sda, INPUT_PULLUP); 
    pinMode(startBtn, INPUT_PULLDOWN);

    start = digitalRead(startBtn);  

    pinMode(menuPot, INPUT); 
    pinMode(menuPlus, INPUT_PULLDOWN); 
    pinMode(menuMinus, INPUT_PULLDOWN); 

    oled.begin(); 
    oled.setFont(SmallFont); 
}

void readFromEEPROM() { 
    p = EEPROM.read(MenuItems::menu_p);
    i = 0; 
    d = EEPROM.read(MenuItems::menu_d);
    gain = EEPROM.read(MenuItems::menu_gain);
    qrdThreshold = EEPROM.read(MenuItems::menu_qrdThreshold); 
    defaultSpeed = EEPROM.read(MenuItems::menu_defaultSpeed);
}

int optionState = 0; 
void pidMenu() { 
    int potVal = analogRead(menuPot) / 10; 
    
    oled.print("PID Menu", 0, 0); 
    oled.print("p: ", 0, 10); 
    oled.print("d: ", 0, 20); 
    oled.print("gain: ", 0, 30); 
    oled.print("thresh: ", 0, 40); 
    oled.print("Speed: ", 0, 50); 

    delay(100); 
    if (!digitalRead(menuPlus)) {
        if (optionState == 4)  {
            optionState = 0;
        }
        else {
            optionState++;
        }
    }
    if (digitalRead(menuMinus)) {toggle = !toggle;}

    oled.printNumI(p, RIGHT, 10);
    oled.printNumI(d, RIGHT, 20);
    oled.printNumI(gain, RIGHT, 30);
    oled.printNumI(pidControl.getLeftQRDReading(), 30, 40);
    oled.printNumI(pidControl.getRightQRDReading(), 60, 40); 
    oled.printNumI(qrdThreshold, RIGHT, 40);
    oled.printNumI(defaultSpeed, RIGHT, 50); 
    oled.print("<--", 45, (optionState+1)*10); 

    if (toggle) { 
        oled.print("Edit", RIGHT, 0); 
    
        switch (optionState) { 
            case 0:
                p = potVal; 
                EEPROM.write(MenuItems::menu_p, p); 
                motorControl.updateP(p);  
                break; 
            case 1: 
                d = potVal; 
                EEPROM.write(MenuItems::menu_d, p); 
                motorControl.updateD(d); 
                break; 
            case 2: 
                gain = potVal; 
                EEPROM.write(MenuItems::menu_gain, p); 
                motorControl.updateGain(gain);
                break; 
            case 3: 
                qrdThreshold = 10*potVal; 
                EEPROM.write(MenuItems::menu_qrdThreshold, p); 
                motorControl.updateThreshold(qrdThreshold);
                break; 
            case 4: 
                defaultSpeed = potVal; 
                EEPROM.write(MenuItems::menu_defaultSpeed, p); 
                motorControl.updateDefaultSpeed(defaultSpeed); 
                motorControl.updateSpeed(defaultSpeed); 
                break; 
            default:
                break; 
        }
    }
    else {
         oled.print("View", RIGHT, 0); 
    }
}

bool initializeFromEEPROM = true; 
void loop() { 
    start = digitalRead(startBtn); 
    if (!start) { 
        if (initializeFromEEPROM) {readFromEEPROM(); initializeFromEEPROM=false;}
        
        oled.clrScr(); 
        pidMenu(); 
        oled.update();  
    }
    else {
        motorControl.poll(); 
        // rightClaw.poll(); 
        // leftClaw.poll(); 
    }
}