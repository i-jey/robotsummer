#include "includes.h"

Basket::Basket(){}; 
Basket::Basket(int servoPin, int basketLim) { 
    this->basketLim = basketLim;
    basketServo.attach(servoPin);
    basketServo.writeMicroseconds(1423); 
}

void Basket::raiseBasket() {
    basketServo.writeMicroseconds(1100); 
}

void Basket::lowerBasket() {
    basketServo.writeMicroseconds(1600); 
}

void Basket::holdBasket() { 
    basketServo.writeMicroseconds(1423); 
}

bool Basket::readBasketSwitch() { 
    return digitalRead(basketLim); 
}