int SENSOR1_PIN = 0;
int SENSOR2_PIN = 1;
int SENSOR3_PIN = 2;
int SENSOR4_PIN = 3;
int SENSOR5_PIN = 4;
unsigned long time;

void setup() {
  Serial.begin(9600);
}

void loop() {
  time = millis();
  // Get input values
  float sensor1_data = analogRead(SENSOR1_PIN);
  float sensor2_data = analogRead(SENSOR2_PIN);
  float sensor3_data = analogRead(SENSOR3_PIN);
  float sensor4_data = analogRead(SENSOR4_PIN);
  float sensor5_data = analogRead(SENSOR5_PIN);

  float all_data[6] = {sensor1_data, sensor2_data, sensor3_data, sensor4_data, sensor5_data};

  // Convert raw data into voltage
  float voltages[6] = {};
  for (int i = 0; i < 5; i++) {
    float voltage = all_data[i] * 5 / 1024;
    voltage = (voltage - 0.5) * 100;
    voltages[i] = voltage;
  }

  // Write to serial
  Serial.print(time);
  Serial.print(",");

  for (int i = 0; i < 5; i++) {
    Serial.print(voltages[i]);
    Serial.print(",");
  }

  Serial.println();

  delay(0);
}
