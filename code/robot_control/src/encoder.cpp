#include "includes.h"

Encoder::Encoder(){}; // Default Constructor
Encoder::Encoder(int encoderLeftPin1, int encoderLeftPin2, int encoderRightPin1, int encoderRightPin2){
    // Encoder Pins must be 5 volt tolerant

    this->encoderLeftPin1 = encoderLeftPin1;
    this->encoderLeftPin2 = encoderLeftPin2;
    this->encoderRightPin1 = encoderRightPin1;
    this->encoderRightPin2 = encoderRightPin2;
    this->countLeft1 = 0;
    this->countLeft2 = 0;
    this->countRight1 = 0;
    this->countRight2 = 0;
}

void Encoder::poll(){
    int currentStateLeft1 = digitalRead(encoderLeftPin1);
    int currentStateLeft2 = digitalRead(encoderLeftPin2);
    int currentStateRight1 = digitalRead(encoderRightPin1);
    int currentStateRight2 = digitalRead(encoderRightPin2);
    Serial.print("Right1: "); Serial.println(currentStateRight1); 
    Serial.print("Right2: "); Serial.println(currentStateRight2); 
    int currentTime = millis();

    if(currentStateLeft1 == HIGH && currentStateLeft1 != prevStateLeft1){
        countLeft1++;
        prevStateLeft1 = currentStateLeft1;
    }
    if(currentStateLeft2 == HIGH && currentStateLeft2 != prevStateLeft2){
        countLeft2++;
        prevStateLeft2 = currentStateLeft2;
    }
    if(currentStateRight1 == HIGH && currentStateRight1 != prevStateRight1){
        countRight1++;
        prevStateRight1 = currentStateRight1;
    }
    if(currentStateRight2 == HIGH && currentStateRight2 != prevStateRight2){
        countRight2++;
        prevStateRight2 = currentStateRight2;
    }
    if((countLeft1 == countLeft2) && countLeft1 >= 6){
        speedLeft = countLeft1/(currentTime-prevTimeLeft);
        prevTimeLeft = currentTime;
    }
    if((countRight1 == countRight2) && countRight1 >= 6){
        speedRight = countRight1/(currentTime-prevTimeRight);
        Serial.println(speedRight); 
        prevTimeRight = currentTime;
    }
}

float Encoder::getDistanceLeft(){
    if(getDirectionLeft() == CW){
        distanceLeft = countLeft1*wheelDiameter;
    }else if(getDirectionLeft() == CCW){
        distanceLeft = countLeft2*wheelDiameter;
    }
    return distanceLeft;
}

float Encoder::getDistanceRight(){
    if(getDirectionRight() == CW){
        distanceRight = 3.14*countRight1*wheelDiameter/24;
    }else if(getDirectionRight() == CCW){
        distanceRight = 3.14*countRight2*wheelDiameter/24;
    }
    return distanceRight;
}

float Encoder::getSpeedLeft(){
    return speedLeft;
}

float Encoder::getSpeedRight(){
    return speedRight;
}

float Encoder::getSpeedOffset(){
    return speedLeft - speedRight;
}


int Encoder::getDirectionLeft(){

    if(countLeft1 > countLeft2){
        directionLeft = CW;
    }else if(countLeft2 > countLeft1) {
        directionLeft = CCW;
    }

    return directionLeft;
}

int Encoder::getDirectionRight(){
    if(countRight1 > countRight2){
        directionRight = CW;
    }else if(countRight2 > countRight1){
        directionLeft = CCW;
    }

    return directionRight;
}


