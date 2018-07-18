#ifndef MOTORINIT_H 
#define MOTORINIT_H 

class MotorInit { 
    private: 
        int highPwmPins[2] = {PA0, PA2}; 
        int lowPwmPins[2] = {PA1, PA3}; 
    public: 
        void init(); 
}; 

#endif 