#include "includes.h"

Basket::Basket(){}; 
Basket::Basket(int servoPin, int basketLim) { 
    this->basketLim = basketLim;
    basketServo.writeMicroseconds(1420); 
}

void Basket::begin() { 
    pinMode(basketLim, INPUT_PULLDOWN); 
    basketServo.attach(servoPin); 
    holdBasket(); 
}

void Basket::raiseBasket() {
    basketServo.writeMicroseconds(1100); 
}

void Basket::lowerBasket() {
    basketServo.writeMicroseconds(1600); 
}

void Basket::holdBasket() { 
    basketServo.writeMicroseconds(1420); 
}

bool Basket::readBasketSwitch() { 
    return digitalRead(basketLim); 
}