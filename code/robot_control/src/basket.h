#ifndef BASKET_H
#define BASKET_H 

class Basket { 
    private:
        Servo basketServo; 
        int servoPin;
        int basketLim;  
    public: 
        Basket(); 
        Basket(int servoPin, int basketLim); 
        void raiseBasket(); 
        void lowerBasket(); 
        void holdBasket(); 
        bool readBasketSwitch(); 
}; 

#endif