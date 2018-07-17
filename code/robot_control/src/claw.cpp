#include "includes.h"
#include "claw.h" 

Arm::Arm(){}; // default constructor otherwise C++ whines
Arm::Arm(int clampPin, int armPin, int pushButton) { 
    clampServo.attach(clampPin); 
    armServo.attach(armPin);  
    this->pushButton = pushButton; 
}

bool Arm::ewokDetected() { 
    if (digitalRead(pushButton)) { 
        return true; 
    }
    return false; 
}

bool Arm::close() {
    clampServo.write(140);
    return true; 
}

bool Arm::open() { 
    clampServo.write(40);    
    return true; 
}

bool Arm::raise() { 
    armServo.write(50);
    Serial.println("RAISE"); 
    return true;
}

bool Arm::lower() { 
    clampServo.write(140); 
    armServo.write(180); 
    Serial.println("LOWER");
    return true; 
}
