// #include <Arduino.h>
// #include <Servo.h> 

// Servo clamp; 
// Servo arm; 

// unsigned long time; 
// unsigned long prevTime = 0; 

// constexpr int sensor1 = PA5; 
// constexpr int sensor2 = PA4; 
// constexpr int POLL_TIME = 50; 

// void setup() {
//     Serial.begin(9600); 
    
//     // Push buttons 
//     pinMode(sensor1, INPUT); 
//     pinMode(sensor2, INPUT); 

//     // Servos
//     clamp.attach(PA1); 
//     arm.attach(PA2);
// }

// int sensorVal = 0; 

// void loop() {
//     sensorVal = digitalRead(sensor1); 
    
//     while (sensorVal) { 
//         clamp.write(180); 
//         delay(POLL_TIME); 
//         sensorVal = digitalRead(sensor1); 
//         // Serial.print("Loop "); 
//         // Serial.print ln(sensorVal); 
//     }

//     clamp.write(0); 
//     delay(POLL_TIME); 
// }