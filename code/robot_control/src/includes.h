#ifndef INCLUDES_H 
#define INCLUDES_H 

// Global variables 
extern int temp;
extern int ewokCounter;  
extern int edgeCounters; 
extern int globalMotorStateTracker; 

// Encoder global variables
constexpr int countsPerRotation = 48; 
constexpr float wheelDiameter = 6.14; // cm
constexpr float wheelToWheelDistance = 10; // cm
extern volatile int leftWheelCounter; 
extern volatile int rightWheelCounter; 

// libraries 
#include <Arduino.h>
#include <Servo.h>
#include <OLED_I2C.h>
#include <EEPROM.h>

// header files 
#include "motor.h"
#include "motorinit.h"
#include "claw.h" 
#include "clawSequence.h"
#include "bridge.h"
#include "tapeFollow.h"
#include "infraredReader.h"
#include "basket.h"
#include "motorControlSequence.h"

#endif 