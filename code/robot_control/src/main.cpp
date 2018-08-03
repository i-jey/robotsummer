#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

constexpr int INSIDE = 1; 
// Global variable initialization 
int ewokCounter = 0; 
int globalClawStateTracker = 0; 
int edgeCounters = 0; 
volatile int leftWheelCounter = 0; 
volatile int rightWheelCounter = 0; 

// EEPROM / menu reference 
enum MenuItems {
    menu_qrdThreshold,
    menu_p,
    menu_d, 
    menu_gain,  
    menu_defaultSpeed, 
    edgeThresh, 
    firstBridgeLowerAngle, 
    firstBridgeUpperAngle, 
    edgeReverseDistance, 
    dropBridgeDistance, 
    driveOverDistance, 
    s3Tilt, 
    s3Reverse, 
    s3Drop, 
    s3Pullback, 
};

// Menu pins 
constexpr int startBtn = PC15;
constexpr int menuMinus = PA12; 
constexpr int menuToggle = PA15; 
constexpr int menuPlus = PA11; 
constexpr int menuPot = PB1; 

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

// Encoder 
constexpr int encoderLeft = PB14; 
constexpr int encoderRight = PB15; 

// PID QRDs 
constexpr int right_pid_QRD = PA4; 
constexpr int left_pid_QRD = PA6; 

// Edge QRDs 
constexpr int right_edge_QRD = PA5; 
constexpr int left_edge_QRD = PA7; 

// Bridge pins 
constexpr int bridgePin1 = PB9; 
constexpr int bridgePin2 = PB2; 

// Right claw pins 
constexpr int right_clamp_pin = PB8; 
constexpr int right_arm_pin = PB7; 
constexpr int right_push_button = PB4; 

// Left claw pins 
constexpr int left_clamp_pin = PB6;
constexpr int left_arm_pin = PB0; 
constexpr int left_push_button = PB3; 

// IR Pins 
constexpr int irPin1k = PB13;
constexpr int irPin10k = PB12;  

// Basket pin 
constexpr int basketPin = PA8; 

// Bridge 
int edgeThreshold = 500; 
int bridge1LowerAngle = 40; 
int bridge1UpperAngle = 130; 
Bridge bridge = Bridge(bridgePin1, bridgePin2, left_edge_QRD, right_edge_QRD, edgeThreshold, bridge1LowerAngle, bridge1UpperAngle); 

// PID constants
int p = 0;
int i = 0; 
int d = 0;
int gain = 0;
int qrdThreshold = 0; 
int defaultSpeed = 120; 

IRReader ir = IRReader(irPin1k, irPin10k); 
Basket basket = Basket(basketPin);
TapeFollow pidControl = TapeFollow(left_pid_QRD, right_pid_QRD); 

// Claw 
int leftClawCloseAngle = 177; 
int leftClawOpenAngle = 60; 
int leftClawOpenAngleInside = 92; 
int leftClawLowerAngle = 25; 
int leftClawRaiseAngle = 160; 
int leftVertical = 136; 

int rightClawCloseAngle = 0; 
int rightClawOpenAngle = 110; 
int rightClawOpenAngleInside = 127; 
int rightClawLowerAngle = 157; 
int rightClawRaiseAngle = 25; 
int rightVertical = 44; 

int closeTime = 250; 
int raiseTime = 1000; 
int openTime = 500; 
int closeTime2 = 500; 
int lowerTime = 1000; 
int resetTime = 1000; 

// Initialize arms and claws 
Arm rightArm = Arm(right_clamp_pin, right_arm_pin, right_push_button, rightClawCloseAngle, rightClawOpenAngle, rightClawOpenAngleInside, rightClawLowerAngle, rightClawRaiseAngle, rightVertical); 
ClawSequence rightClaw = ClawSequence(rightArm, closeTime, raiseTime, openTime, closeTime2, lowerTime, resetTime); 
Arm leftArm = Arm(left_clamp_pin, left_arm_pin, left_push_button, leftClawCloseAngle, leftClawOpenAngle, leftClawOpenAngleInside, leftClawLowerAngle, leftClawRaiseAngle, leftVertical); 
ClawSequence leftClaw = ClawSequence(leftArm, closeTime, raiseTime, openTime, closeTime2, lowerTime, resetTime); 

// Initialize motor
Motor rightMotor = Motor(right_motor_pin1, right_motor_pin2); 
Motor leftMotor = Motor(left_motor_pin1, left_motor_pin2); 
MotorControl motorControl = MotorControl(leftMotor, rightMotor, bridge, ir, basket, 
    pidControl, leftClaw, rightClaw, qrdThreshold, gain, p, i, d); 
MotorInit motorInit = MotorInit(); 
int globalMotorStateTracker = 0; 

// Menu bools 
bool start; 
bool toggle = false; 

unsigned long leftEncoderDebounce = 0; 
unsigned long rightEncoderDebounce = 0; 

void leftEncoderIncrement() { 
    // Serial.println("LEFT TRIGGERED"); 
    // if (millis() > leftEncoderDebounce) { 
    //     leftWheelCounter++; 
    //     leftEncoderDebounce = millis() + 10; 
    // }
    leftWheelCounter++; 
}
void rightEncoderIncrement() { 
    // if (millis() > rightEncoderDebounce) { 
    //     rightWheelCounter++; 
    //     rightEncoderDebounce = millis() + 10; 
    // }
    rightWheelCounter++;
}

void setup() {  
    Serial.begin(9600); 
    
    // Encoder interrupts 
    pinMode(encoderLeft, INPUT_PULLDOWN); 
    pinMode(encoderRight, INPUT_PULLDOWN); 
    attachInterrupt(encoderLeft, leftEncoderIncrement, RISING); 
    attachInterrupt(encoderRight, rightEncoderIncrement, RISING); 

    // Initialize motor pins 
    motorInit.init(); 

    // Initialize qrds 
    pinMode(left_edge_QRD, INPUT); 
    pinMode(right_edge_QRD, INPUT); 
    pinMode(left_pid_QRD, INPUT); 
    pinMode(right_pid_QRD, INPUT); 

    // Initialize claw buttons
    pinMode(right_push_button, INPUT_PULLDOWN); 
    pinMode(left_push_button, INPUT_PULLDOWN); 
    rightArm.open(false); 
    leftArm.open(false); 
    rightArm.lower(); 
    leftArm.lower(); 

    // IR 
    pinMode(irPin1k, INPUT_PULLDOWN); 
    pinMode(irPin10k, INPUT_PULLDOWN); 

    // Menu dials and OLED
    pinMode(oled_scl, INPUT_PULLUP); 
    pinMode(oled_sda, INPUT_PULLUP); 
    pinMode(startBtn, INPUT_PULLDOWN);
    pinMode(menuPlus, INPUT_PULLDOWN); 
    pinMode(menuToggle, INPUT_PULLDOWN);
    pinMode(menuMinus, INPUT_PULLDOWN); 
    pinMode(menuPot, INPUT); 
    start = digitalRead(startBtn);  

    oled.begin(); 
    oled.setFont(SmallFont); 
}

void writeToEEPROM(int loc, int val) {
    EEPROM.write(loc*2+1, val&255);
    EEPROM.write(loc*2, val>>8);  
}

int16_t readFromEEPROM(int loc) { 
    return (EEPROM.read(loc*2)<<8)|EEPROM.read(loc*2+1);
}

void initializeFromEEPROM() { 
    // Motor control values
    p = readFromEEPROM(MenuItems::menu_p); motorControl.updateP(p); 
    i = 0; motorControl.updateI(i);
    d = readFromEEPROM(MenuItems::menu_d); motorControl.updateD(d); 
    gain = readFromEEPROM(MenuItems::menu_gain); motorControl.updateGain(gain);
    qrdThreshold = readFromEEPROM(MenuItems::menu_qrdThreshold); motorControl.updateThreshold(qrdThreshold);
    defaultSpeed = readFromEEPROM(MenuItems::menu_defaultSpeed); motorControl.updateDefaultSpeed(defaultSpeed);
    motorControl.edgeReverseDistance = readFromEEPROM(MenuItems::edgeReverseDistance); 
    motorControl.dropBridgeDistance = readFromEEPROM(MenuItems::dropBridgeDistance); 
    motorControl.driveOverDistance = readFromEEPROM(MenuItems::driveOverDistance); 

    // Bridge values 
    edgeThreshold = readFromEEPROM(MenuItems::edgeThresh); motorControl.updateEdgeThreshold(edgeThreshold);
    // bridge1LowerAngle = readFromEEPROM(MenuItems::firstBridgeLowerAngle); bridge.updateFirstBridgeLowerAngle(bridge1LowerAngle);
    // bridge1UpperAngle = readFromEEPROM(MenuItems::firstBridgeUpperAngle); bridge.updateFirstBridgeUpperAngle(bridge1UpperAngle);

    // Default values after flashing 
    if (p == -1) {p = 4; motorControl.updateP(p);}
    if (d == -1) {d = 5; motorControl.updateD(d);}
    if (gain == -1) {gain = 13; motorControl.updateGain(gain);}
    if (defaultSpeed == -1) {defaultSpeed = 180; motorControl.updateDefaultSpeed(defaultSpeed);}

    if (motorControl.edgeReverseDistance == -1) {motorControl.edgeReverseDistance = 75;}
    if (motorControl.dropBridgeDistance == -1) {motorControl.dropBridgeDistance = 250;}
    if (motorControl.driveOverDistance == -1) {motorControl.driveOverDistance = 1000;}
    if (edgeThreshold == -1) {edgeThreshold = 500; motorControl.updateEdgeThreshold(edgeThreshold);}
    if (qrdThreshold == -1) {qrdThreshold = 900; motorControl.updateThreshold(qrdThreshold);}

    if (motorControl.s3TiltLeftTime == -1) { motorControl.s3TiltLeftTime = 100;}
    if (motorControl.s3ReverseTime == -1) { motorControl.s3ReverseTime = 250;}
    if (motorControl.s3LeftPullBackTime == -1) {motorControl.s3LeftPullBackTime = 100;}
}

int optionState = 0; 
void pidMenu() { 
    int potVal = analogRead(menuPot) / 10; 
    
    oled.print("PID Menu", 0, 0); 
    oled.print("thresh: ", 0, 10); 
    oled.print("p: ", 0, 20); 
    oled.print("d: ", 0, 30); 
    oled.print("gain: ", 0, 40); 
    oled.print("Speed: ", 0, 50); 

    delay(125); // Debouncing delay 

    if (digitalRead(menuPlus)) {
        if (optionState >= 4)  {
            optionState = 0;
        }
        else {
            optionState++;
        }
    }
    // edit-view toggle 
    if (digitalRead(menuMinus)) {toggle = !toggle;}

    oled.printNumI(pidControl.getLeftQRDReading(), 45, 10);
    oled.printNumI(pidControl.getRightQRDReading(), 75, 10); 
    oled.printNumI(qrdThreshold, RIGHT, 10);
    oled.printNumI(p, RIGHT, 20);
    oled.printNumI(d, RIGHT, 30);
    oled.printNumI(gain, RIGHT, 40); 
    oled.printNumI(defaultSpeed, RIGHT, 50); 
    oled.print("<--", 45, (optionState+1)*10); 

    if (toggle) { 
        oled.print("Edit", RIGHT, 0); 
    
        switch (optionState) { 
            case MenuItems::menu_p:
                p = potVal; 
                writeToEEPROM(MenuItems::menu_p, p); 
                motorControl.updateP(p);
                break; 
            case MenuItems::menu_d: 
                d = potVal; 
                writeToEEPROM(MenuItems::menu_d, d); 
                motorControl.updateD(d); 
                break; 
            case MenuItems::menu_gain: 
                gain = potVal; 
                writeToEEPROM(MenuItems::menu_gain, gain); 
                motorControl.updateGain(gain);
                break; 
            case MenuItems::menu_qrdThreshold: 
                qrdThreshold = 10*potVal; 
                writeToEEPROM(MenuItems::menu_qrdThreshold, qrdThreshold); 
                motorControl.updateThreshold(qrdThreshold);
                pidControl.updateThreshold(qrdThreshold); 
                break; 
            case MenuItems::menu_defaultSpeed: 
                defaultSpeed = potVal; 
                writeToEEPROM(MenuItems::menu_defaultSpeed, defaultSpeed); 
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

void bridgeMenu() { 
    int potVal = analogRead(menuPot); 

    oled.print("Bridge Menu", 0, 0); 
    oled.print("Thresh: ", 0, 10); 
    oled.print("Reverse1 time[ms]: ", 0, 20); 
    oled.print("Reverse2 time[ms]: ", 0, 30); 
    oled.print("Over time[ms]: ", 0, 40); 
    oled.print("<--", 45, (optionState+1)*10); 

    delay(125); // Debouncing delay 

    if (digitalRead(menuPlus)) { 
        if (optionState >= 3) { 
            optionState = 0; 
        }
        else { 
            optionState++; 
        }
    }

    // edit-view toggle 
    if (digitalRead(menuMinus)) {toggle = !toggle;}

    // Print current values 
    oled.printNumI(edgeThreshold, RIGHT, 10); 
    oled.printNumI(bridge.getLeftEdgeReading(), 45, 10); 
    oled.printNumI(bridge.getRightEdgeReading(), 75, 10); 
    oled.printNumI(motorControl.edgeReverseDistance, RIGHT, 20); 
    oled.printNumI(motorControl.dropBridgeDistance, RIGHT, 30); 
    oled.printNumI(motorControl.driveOverDistance, RIGHT, 40); 
    
    if (toggle) { 
        oled.print("Edit", RIGHT, 0); 
    
        switch (optionState) { 
            case 0:
                edgeThreshold = (potVal / 10) * 10; 
                writeToEEPROM(MenuItems::edgeThresh, edgeThreshold); 
                motorControl.updateEdgeThreshold(edgeThreshold); 
                break; 
            case 1: 
                motorControl.edgeReverseDistance = potVal; 
                writeToEEPROM(MenuItems::edgeReverseDistance, motorControl.edgeReverseDistance); 
                break; 
            case 2: 
                motorControl.dropBridgeDistance = potVal; 
                writeToEEPROM(MenuItems::dropBridgeDistance, motorControl.dropBridgeDistance);
                break; 
            case 3: 
                motorControl.driveOverDistance = potVal; 
                writeToEEPROM(MenuItems::driveOverDistance, motorControl.driveOverDistance);                break; 
            default:
                break; 
        }
    }
    else {
         oled.print("View", RIGHT, 0); 
    }
}

void postTrooperMenu() { 
    int potVal = analogRead(menuPot); 

    oled.print("Post Stormtrooper Menu", 0, 0); 
    oled.print("Tilt left: ", 0, 10);
    oled.print("Reverse: ", 0, 20); 
    oled.print("Drop ewok: ", 0, 30); 
    oled.print("Left pull back: ", 0, 40); 
    oled.print("<--", 45, (optionState+1)*10); 

    delay(125); // debouncing delay

    if (digitalRead(menuPlus)) { 
        if (optionState >= 3) { 
            optionState = 0; 
        }
        else { 
            optionState++; 
        }
    }

    // edit-view toggle
    if (digitalRead(menuMinus)) {toggle = !toggle;}

    // Print current values
    oled.printNumI(motorControl.s3TiltLeftTime, RIGHT, 10); 
    oled.printNumI(motorControl.s3ReverseTime, RIGHT, 20);
    oled.printNumI(motorControl.dropEwokTime, RIGHT, 30); 
    oled.printNumI(motorControl.s3LeftPullBackTime, RIGHT, 40); 


    if (toggle) { 
        oled.print("Edit", RIGHT, 0); 

        switch (optionState) { 
            case 0: 
                motorControl.s3TiltLeftTime = potVal; 
                writeToEEPROM(MenuItems::s3Tilt, motorControl.s3TiltLeftTime); 
                break; 
            case 1: 
                motorControl.s3ReverseTime = potVal; 
                writeToEEPROM(MenuItems::s3Reverse, motorControl.s3ReverseTime); 
                break; 
            case 2: 
                motorControl.dropEwokTime = potVal; 
                writeToEEPROM(MenuItems::s3Drop, motorControl.dropEwokTime); 
                break; 
            case 3: 
                motorControl.s3LeftPullBackTime = potVal; 
                writeToEEPROM(MenuItems::s3Pullback, motorControl.s3LeftPullBackTime); 
                break; 
        default: 
            break; 
        }
    }
    else { 
        oled.print("View", RIGHT, 0); 
    }
}

bool initialize = true; 
int switchMenus = 0; 
bool initMotors = true; 
void loop() { 
    start = digitalRead(startBtn); 
    if (!start) { 
        // Both claws up 
        leftArm.close(); leftArm.verticalRaise(); 
        rightArm.close(); rightArm.verticalRaise(); 

        if (initialize) {
            initializeFromEEPROM(); 
            initialize = false;
        }
        if (digitalRead(menuToggle)) {
            if (switchMenus == 2) {
                switchMenus = 0;
            }
            else { 
                switchMenus++;
            }
            optionState = 0;
        }
        oled.clrScr(); 
        
        if (switchMenus == 0) { 
            pidMenu(); 
        }
        else if (switchMenus == 1) { 
            bridgeMenu();   
        }
        else if (switchMenus == 2) { 
            postTrooperMenu(); 
        }

        oled.update();  
        initMotors = true; 
    }
    else {
        if (initMotors) {
            initMotors = false; 

            motorInit.init();

            oled.clrScr(); 
            oled.print("PL2 W0RK", 40, 50); 
            oled.update(); 
            
            // reset global values 
            ewokCounter = 0; 
            globalClawStateTracker = 2; rightClaw.reset(); rightArm.lower(); rightArm.open(!INSIDE); 
            edgeCounters = 0; 
            leftWheelCounter = 0; 
            rightWheelCounter = 0; 
            motorControl.reset(); 
            bridge.raiseBoth(); 
            // grace period before it starts to go 
            // oled.print("3", 60, 30); oled.update(); 
            // delay(1000); 
            oled.print("2", 60, 30); oled.update(); 
            delay(1000); 
            oled.print("1", 60, 30); oled.update(); 
            delay(1000); 

            motorControl.stateOverride(2, 0); // state 0 = continuous forward drive
            leftClaw.stateOverride(10); 
        }
        
        oled.clrScr();
        oled.print("PL2 W0RK", 40, 0); 
        oled.print("Current state: ", 0, 20);
        oled.printNumI(globalMotorStateTracker, RIGHT, 20);  
        oled.print("Claw state: ", 0, 30);
        oled.printNumI(globalClawStateTracker, RIGHT, 30);  
        oled.print("Ewok counter: ", 0, 40);
        oled.printNumI(ewokCounter, RIGHT, 40); 
        oled.printNumI(digitalRead(irPin1k), 50, 50); 
        oled.printNumI(digitalRead(irPin10k), 80, 50); 
        oled.update(); 
        motorControl.poll(); 
        rightClaw.poll(); 
        leftClaw.poll(); 

        if (globalClawStateTracker == 0) {
            rightArm.lower(); leftArm.lower(); 
            rightArm.open(!INSIDE); leftArm.open(!INSIDE); 
            rightClaw.stateOverride(0);
            leftClaw.stateOverride(0);  
        }
        else if (globalClawStateTracker == 1) { 
            // right up
            rightClaw.stateOverride(10); 
        }
        else if (globalClawStateTracker == 2) { 
            // left up 
            leftClaw.stateOverride(10); 
        }
        else if (globalClawStateTracker == 3) { 
            // both up
            leftClaw.stateOverride(10); 
            rightClaw.stateOverride(10); 
        }
        else if (globalClawStateTracker == 4) { 
            // left down, right up, don't start polling just yet  
            leftClaw.stateOverride(11); 
            rightClaw.stateOverride(10); 
        }
        else if (globalClawStateTracker == 5) { 
            // left claw start polling
            leftClaw.stateOverride(0); 
        }
        globalClawStateTracker = 99; 
    }
}