#ifndef ENCODER_H
#define ENCODER_H

constexpr int CW = 1;
constexpr int CCW = -1; 

void encoderInterruptHandlerLeft1(){
    
}


class Encoder {
    private:
        int encoderLeftPin1;
        int encoderLeftPin2;
        int encoderRightPin1;
        int encoderRightPin2;
        volatile int countLeft1;
        volatile int countLeft2;
        volatile int countRight1;
        volatile 
        int prevTime;
        int direction;
        float speed;
        
    public:
        Encoder();
        Encoder(int encoderPin1, int encoderPin2);
        void encoderInterruptHandler1();
        void encoderInterruptHandler2();
        float getSpeed();
        bool getDirection();
};



#endif