#include "includes.h" 

void MotorInit::init() { 
    for (int x : this->highPwmPins) {
         analogWrite(0, x); 
         pinMode(x, OUTPUT); 
    }
    delay(50); 
    for (int x : this->lowPwmPins) { 
        analogWrite(0, x); 
        pinMode(x, OUTPUT); 
    }
}
