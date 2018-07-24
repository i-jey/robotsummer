#ifndef MOTOR_H
#define MOTOR_H

class Motor { 
    private: 
        int motorPin1;
        int motorPin2;  

    public: 
        Motor();
        Motor(int motorPin1, int motorPin2); 
        void forward(int speed); 
        void reverse(int speed); 
        void write(int speed); 
};

#endif