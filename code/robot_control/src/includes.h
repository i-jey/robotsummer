#ifndef INCLUDES_H 
#define INCLUDES_H 

// Global variables 
extern int prevEwokCounter; 
extern int ewokCounter;  
extern int edgeCounters; 
extern int globalMotorStateTracker; 
extern int globalClawStateTracker; 
extern bool raiseBasket; 
extern bool holdBasket; 
extern bool lowerBasket; 

// Claw 
extern int rightClawState; 
extern int leftClawState; 

// libraries 
#include <Arduino.h>
#include <Servo.h>
#include <OLED_I2C.h>
#include <EEPROM.h>

// header files 
#include "basket.h"
#include "bridge.h"
#include "motor.h"
#include "motorinit.h"
#include "claw.h" 
#include "clawSequence.h"
#include "tapeFollow.h"
#include "infraredReader.h"
#include "motorControlSequence.h"
// #include "menu.h"

#endif 