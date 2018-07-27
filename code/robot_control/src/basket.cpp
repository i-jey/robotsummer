#include "includes.h"

Basket::Basket(){}; 
Basket::Basket(int basketPin) { 
    this->basketPin = basketPin; 
}

void Basket::raiseBasket() { 
    digitalWrite(basketPin, HIGH); 
}

void Basket::lowerBasket() { 
    digitalWrite(basketPin, LOW);
}