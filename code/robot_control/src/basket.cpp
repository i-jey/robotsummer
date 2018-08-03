#include "includes.h"

Basket::Basket(){}; 
Basket::Basket(int servoPin, int basketLim) { 
    this->basketLim = basketLim;
    basketServo.attach(servoPin);
}

void Basket::raiseBasket() {
    basketServo.writeMicroseconds(1600); 
    Serial.println("Raising basket"); 
}

void Basket::lowerBasket() {
    basketServo.writeMicroseconds(1100); 
}

void Basket::holdBasket() { 
    basketServo.writeMicroseconds(1430); 
}

bool Basket::readBasketSwitch() { 
    return digitalRead(basketLim); 
}