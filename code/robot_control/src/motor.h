#ifndef MOTOR_H
#define MOTOR_H

class Motor { 
    private: 
        int motorPin; 

    public: 
        Motor();
        Motor(int motorPin); 
        void forward(int speed); 
        void reverse(int speed); 
};

#endif