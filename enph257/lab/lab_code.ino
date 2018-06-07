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

int FLIPPING_SWITCH = 7;
unsigned long HEATING_TIME = 350000;
unsigned long END_TIME = 7200000; 

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
  SD.remove("ENPH257/AAAAAA.txt"); 
  myFile = SD.open("ENPH257/AAAAAA.txt", FILE_WRITE);
  myFile.println("Time(ms),Temp1,Temp2,Temp3,Temp4,Temp5,Ambient");

  // Start with heating 
  digitalWrite(FLIPPING_SWITCH, HIGH); 
}

void loop() {
  if (myFile) { 
    Serial.println("Writing to file..."); 
    time = millis();
    
    if (time - prevTime > HEATING_TIME && digitalRead(FLIPPING_SWITCH) == 0) {
      digitalWrite(FLIPPING_SWITCH, HIGH);
      Serial.println("HIGH"); 
      prevTime = time;
    }
    else if (time - prevTime > HEATING_TIME && digitalRead(FLIPPING_SWITCH)) {
      digitalWrite(FLIPPING_SWITCH, LOW);
      Serial.println("LOW");
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
  
    // Write to file
    myFile.print(time);
    myFile.print(",");
  
    for (int i = 0; i < 6; i++) {
      myFile.print(voltages[i]);
      if (i != 5){ 
        myFile.print(",");
      }
      Serial.print(voltages[i]);
      Serial.print(",");
    }
    Serial.println();
    myFile.println();
    
    if (time > END_TIME) {
      myFile.close();
    }
    
    delay(100);
  }
  else { 
    if (digitalRead(FLIPPING_SWITCH)) { 
      digitalWrite(FLIPPING_SWITCH, LOW); 
    }
    Serial.println("error opening file OR completed writing"); 
  }
}
