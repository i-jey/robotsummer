#ifndef BASKET_H
#define BASKET_H 

class Basket { 
    private:
        int basketPin; 
    public: 
        Basket(); 
        Basket(int basketPin); 
        void raiseBasket(); 
        void lowerBasket(); 
}; 

#endif