#include <SPI.h>
#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 4;
File myFile;

int SENSOR1_PIN = 0;
int SENSOR2_PIN = 1;
int SENSOR3_PIN = 2;
int SENSOR4_PIN = 3;
int SENSOR5_PIN = 4;
int SENSOR6_PIN = 5;

int FLIPPING_SWITCH = 13;
int DUTY_CYCLE = 6000;
int END_TIME = 15 * 1000; 

unsigned long time;
unsigned long prevTime;

void setup() {
  Serial.begin(9600);
  pinMode(FLIPPING_SWITCH, OUTPUT);

  Serial.print("Initializing SD card..."); 

  if (!SD.begin(4)) { 
    Serial.println("Initialization failed"); 
    while(1); 
  }
  Serial.println("Initialization success"); 
  myFile = SD.open("ENPH257/AAAAAA.txt", FILE_WRITE);
  myFile.println("Time(ms),Temp1,Temp2,Temp3,Temp4,Temp5,Ambient");
}

void loop() {
  if (myFile) { 
    Serial.println("Writing to file..."); 
    time = millis();
  
    // Toggle the heater every 6 seconds
    if (time - prevTime > DUTY_CYCLE && digitalRead(FLIPPING_SWITCH) == 0) {
      digitalWrite(FLIPPING_SWITCH, HIGH);
      prevTime = time;
    }
    else if (time - prevTime > DUTY_CYCLE && digitalRead(FLIPPING_SWITCH)) {
      digitalWrite(FLIPPING_SWITCH, LOW);
      prevTime = time;
    }
  
    // Get input values
    float sensor1_data = analogRead(SENSOR1_PIN);
    float sensor2_data = analogRead(SENSOR2_PIN);
    float sensor3_data = analogRead(SENSOR3_PIN);
    float sensor4_data = analogRead(SENSOR4_PIN);
    float sensor5_data = analogRead(SENSOR5_PIN);
    float sensor6_data = analogRead(SENSOR6_PIN);
  
    float all_data[6] = {sensor1_data, sensor2_data, sensor3_data, sensor4_data, sensor5_data, sensor6_data};
  
    // Convert raw data into voltage
    float voltages[6] = {};
    for (int i = 0; i < 6; i++) {
      float voltage = all_data[i] * 5 / 1024;
      voltage = (voltage - 0.5) * 100;
      voltages[i] = voltage;
    }
  
    // Write to serial
    myFile.print(time);
    myFile.print(",");
    
//    Serial.print(time);
//    Serial.print(",");
  
    for (int i = 0; i < 6; i++) {
//      Serial.print(voltages[i]);
//      Serial.print(",");
  
      myFile.print(voltages[i]);
      if (i != 5){ 
        myFile.print(",");
      }
    }
    
    myFile.println();
//    Serial.println();
  
    if (time > END_TIME) { 
      myFile.close();
    }
    
    delay(1000);
  }
  else { 
    if (digitalRead(FLIPPING_SWITCH)) { 
      digitalWrite(FLIPPING_SWITCH, LOW); 
    }
    Serial.println("error opening file OR completed writing"); 
  }
}
