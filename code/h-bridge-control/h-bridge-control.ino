#include <phys253.h>
#include <LiquidCrystal.h>

const int motor_pin = 2;

void setup() {
  #include <phys253setup.txt>

  Serial.begin(9600);
  LCD.clear();
  LCD.setCursor(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  /** 
   *              Hb1 - Hb2 
   *  STOP        LOW - LOW
   *  FORWARD     HIGH - LOW
   *  REVERSE     LOW - HIGH
   *  NOT ALLOWED HIGH - HIGH
 */
 
  float val = knob(6); 
  float out = 0.5 * val - 255; 

  motor.speed(motor_pin, out);

  LCD.setCursor(0, 0); 
  LCD.print("Knob: ");
  LCD.print(val); 
  LCD.setCursor(0, 1); 
  LCD.print("Speed: "); 
  LCD.print(out); 
}
