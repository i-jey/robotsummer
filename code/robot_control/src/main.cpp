#include "includes.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

// Temp global variable
int temp = 0; 
int ewokCounter = 0; 
int edgeCounters = 0; 
volatile int leftWheelCounter = 0; 
volatile int rightWheelCounter = 0; 

// EEPROM / menu reference 
enum MenuItems { 
    menu_p,
    menu_d, 
    menu_gain, 
    menu_qrdThreshold, 
    menu_defaultSpeed, 
    edgeThresh, 
    firstBridgeLowerAngle, 
    firstBridgeUpperAngle, 
    edgeReverseDistance, 
    dropBridgeDistance, 
    driveOverDistance, 
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
constexpr int left_clamp_pin = PB0;
constexpr int left_arm_pin = PB6; 
constexpr int left_push_button = PB3; 

// IR Pins 
constexpr int irPin1 = PB12; 
constexpr int irPin2 = PB13; 

// Basket pin 
constexpr int basketPin = PA8; 

// Bridge 
int edgeThreshold = 500; 
int bridge1LowerAngle = 47; 
int bridge1UpperAngle = 110; 
Bridge bridge = Bridge(bridgePin1, bridgePin2, left_edge_QRD, right_edge_QRD, edgeThreshold, bridge1LowerAngle, bridge1UpperAngle); 

// PID constants
int p = 0;
int i = 0; 
int d = 0;
int gain = 0;
int qrdThreshold = 0; 
int defaultSpeed = 120; 

IRReader ir = IRReader(irPin1, irPin2); 
Basket basket = Basket(basketPin);
TapeFollow pidControl = TapeFollow(left_pid_QRD, right_pid_QRD); 

// Claw 
int leftClawCloseAngle = 140; 
int leftClawOpenAngle = 50; 
int leftClawOpenAngleInside; 
int leftClawLowerAngle = 50; 
int leftClawRaiseAngle = 180; 

int rightClawCloseAngle = 0; 
int rightClawOpenAngle = 110; 
int rightClawOpenAngleInside = 127; 
int rightClawLowerAngle = 133; 
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
    if (millis() > leftEncoderDebounce) { 
        leftWheelCounter++; 
        leftEncoderDebounce = millis() + 10; 
    }
}
void rightEncoderIncrement() { 
    if (millis() > rightEncoderDebounce) { 
        rightWheelCounter++; 
        rightEncoderDebounce = millis() + 10; 
    }
}

void setup() {  
    Serial.begin(9600); 
    
    // Encoder interrupts 
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
    edgeThreshold = readFromEEPROM(MenuItems::edgeThresh); bridge.updateThreshold(edgeThreshold);
    bridge1LowerAngle = readFromEEPROM(MenuItems::firstBridgeLowerAngle); bridge.updateFirstBridgeLowerAngle(bridge1LowerAngle);
    bridge1UpperAngle = readFromEEPROM(MenuItems::firstBridgeUpperAngle); bridge.updateFirstBridgeUpperAngle(bridge1UpperAngle);
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

    delay(100); // Debouncing delay 

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

int erd; 
int dbd; 
int dod; 
void bridgeMenu() { 
    int potVal = analogRead(menuPot); 

    oled.print("Bridge Menu", 0, 0); 
    oled.print("Thresh: ", 0, 10); 
    oled.print("Reverse from edge counts: ", 0, 20); 
    oled.print("drop bridge reverse counts: ", 0, 30); 
    oled.print("drive over counts: ", 0, 40); 
    oled.print("<--", 45, (optionState+1)*10); 

    delay(100); // Debouncing delay 

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

    // Display count --> distance conversion 
    oled.printNumI(motorControl.edgeReverseDistance * wheelDiameter * 3.14159265, 100, 20); 
    oled.printNumI(motorControl.dropBridgeDistance * wheelDiameter * 3.14159265, 100, 30); 
    oled.printNumI(motorControl.driveOverDistance * wheelDiameter * 3.14159265, 100, 40);
    
    if (toggle) { 
        oled.print("Edit", RIGHT, 0); 
    
        switch (optionState) { 
            case 0:
                edgeThreshold = potVal; 
                writeToEEPROM(MenuItems::edgeThresh, edgeThreshold); 
                bridge.updateThreshold(edgeThreshold); 
                break; 
            case 1: 
                erd = potVal; 
                writeToEEPROM(MenuItems::edgeReverseDistance, erd); 
                motorControl.edgeReverseDistance = erd; 
                break; 
            case 2: 
                dbd = potVal; 
                writeToEEPROM(MenuItems::dropBridgeDistance, dbd); 
                motorControl.dropBridgeDistance = dbd; 
                break; 
            case 3: 
                dod = potVal; 
                writeToEEPROM(MenuItems::driveOverDistance, dod); 
                motorControl.driveOverDistance = dod; 
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
bool switchMenus = false; 
bool initMotors = true; 
void loop() { 
    start = digitalRead(startBtn); 
    if (!start) { 
        if (initialize) {
            initializeFromEEPROM(); 
            initialize = false;
        }
        if (digitalRead(menuToggle)) {switchMenus = !switchMenus;}
        oled.clrScr(); 
        
        if (switchMenus) { 
            pidMenu(); 
        }
        else { 
            bridgeMenu();   
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
            
            // grace period before it starts to go 
            oled.print("3", 50, 0); oled.update(); 
            delay(1000); 
            oled.print("2", 50, 0); oled.update(); 
            delay(1000); 
            oled.print("1", 50, 0); oled.update(); 
            delay(1000); 

            motorControl.stateOverride(100, 0); // state 20 = rotate left 90, right 90
            ewokCounter = 0; 
            edgeCounters = 0; 
            leftWheelCounter = 0; 
            rightWheelCounter = 0; 
        }

        oled.clrScr();
        oled.print("Current state: ", 0, 30);
        oled.printNumI(globalMotorStateTracker, 100, 30);  
        oled.printNumI(leftWheelCounter, 40, 50); 
        oled.printNumI(rightWheelCounter, 80, 50); 
        oled.update(); 
        motorControl.poll(); 
        rightClaw.poll(); 
        leftClaw.poll(); 

        if (temp==1) {
            // Change to bridge sequence motor state
            motorControl.stateOverride(10, 1000);
            temp = 0;
        }
    }
}