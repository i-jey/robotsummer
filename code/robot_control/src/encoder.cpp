#include "includes.h"

Encoder::Encoder(){}; // Default Constructor

Encoder::Encoder(int encoderPin1, int encoderPin2){
    // Encoder Pins must be 5 volt tolerant

    this->encoderPin1 = encoderPin1;
    this->encoderPin2 = encoderPin2;
    this->count1 = 0;
    this->count2 = 0;
    this->prevTime = millis();
}

void Encoder::encoderInteruptHandler1(){
    count1++;
}

void Encoder::encoderInteruptHandler2(){
    count2++;
}

float Encoder::getSpeed(){
    int currentTime = millis();
    int timeDifference = prevTime - currentTime;

    if(getDirection){
        speed = count1/timeDifference;
    }else{
        speed = count2/timeDifference;
    }
    prevTime = currentTime;

    return speed;
}

bool Encoder::getDirection(){
    if(count1 > count2){
        direction = CW;
    }
    else if(count2 > count1) {
        direction = CCW;
    }

    return direction;
}



