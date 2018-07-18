#include "includes.h" 
#include "motorinit.h" 

int highPwmPins[2] = {PA0, PA2}; 
int lowPwmPins[2] = {PA1, PA3}; 

void init() { 
    for (int x : highPwmPins) {
         analogWrite(0, x); 
         pinMode(x, OUTPUT); 
    }
    delay(50); 
    for (int x : lowPwmPins) { 
        analogWrite(0, x); 
        pinMode(x, OUTPUT); 
    }
}
