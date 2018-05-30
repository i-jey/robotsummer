#include <phys253.h>
#include <LiquidCrystal.h>

const int QRD_LEFT_PIN = 0;
const int QRD_RIGHT_PIN = 5;
const int motor_left = 0;
const int motor_right = 1;
boolean start_flag = 0;

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);

}

void loop() { 

  int left;
  int right;

  int left_motor_speed = 200;
  int right_motor_speed = 200;

  if(startbutton() || start_flag){
    start_flag = 1;

    motor.speed(motor_left,left_motor_speed);
    motor.speed(motor_right,right_motor_speed);

    left = analogRead(QRD_LEFT_PIN);
    right = analogRead(QRD_RIGHT_PIN);

    // Turn right 
    if(left - right > 5){
      left_motor_speed -= 20;
      right_motor_speed = 200;
    }
    // Turn left 
    else if(right - left > 5){
      right_motor_speed -=20;
      left_motor_speed = 200; 
    }
    else {
      left_motor_speed = 200;
      right_motor_speed = 200;  
    }
    LCD.clear();
    LCD.setCursor(0,0); LCD.print(left);
    LCD.setCursor(8,0); LCD.print(left_motor_speed);
    LCD.setCursor(0,1); LCD.print(right);
    LCD.setCursor(8,1); LCD.print(right_motor_speed);
    delay(200);
  }else{
    LCD.clear(); LCD.home();
    LCD.print("Push Start");
    LCD.setCursor(0,1);
    LCD.print("When Ready");
    delay(200);   
  }

  if(stopbutton()){
    start_flag = 0;
    motor.stop_all();
  }
}
