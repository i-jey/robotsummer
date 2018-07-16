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
        int countLeft1;
        int countLeft2;
        int countRight1;
        int countRight2; 
        int prevStateLeft1;
        int prevStateLeft2;
        int prevStateRight1;
        int prevStateRight2;
        int prevTimeLeft;
        int prevTimeRight;
        float distanceLeft;
        float distanceRight;
        float speedLeft;
        float speedRight;
        int directionLeft;
        int directionRight;
        
    public:
        Encoder();
        Encoder(int encoderLeftPin1, int encoderLeftPin2, int encoderRightPin1, int encoderRightPin2);
        void poll();
        float getDistanceLeft();
        float getDistanceRight();
        float getSpeedLeft();
        float getSpeedRight();
        int getDirectionLeft();
        int getDirectionRight();
        float getSpeedOffset();
};

#endif