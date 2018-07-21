#ifndef INCLUDES_H 
#define INCLUDES_H 

// Global variables 
extern int temp; 
constexpr float wheelDiameter = 6.14;

// libraries 
#include <Arduino.h>
#include <Servo.h>
#include <OLED_I2C.h>
#include <EEPROM.h>

// header files 
#include "motor.h"
#include "motorinit.h"
#include "motorControlSequence.h"
#include "claw.h" 
#include "clawSequence.h"
#include "bridge.h"
#include "bridgeSequence.h"
#include "encoder.h"
#include "tapeFollow.h"

#endif 