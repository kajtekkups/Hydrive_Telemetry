//Arduino Uno Rev3 obsługujące czujniki Halla, zczytujące pomiar z supercapów -> przekazanie tych informacji do drugiego Arduino oraz do ESP

#include <Arduino.h>

const int hallSensorPin1 = 2;
const int hallSensorPin2 = 3;
const int voltageSensorPin = A0;

volatile unsigned long lastPulseTime1 = 0;
volatile unsigned long lastPulseTime2 = 0;
volatile unsigned long deltaTime1 = 0;
volatile unsigned long deltaTime2 = 0;

float currentVelocity1 = 0;
float currentVelocity2 = 0;
float voltage = 0;

void hallSensor1ISR() {
  unsigned long now = micros();
  deltaTime1 = now - lastPulseTime1;
  lastPulseTime1 = now;
}

void hallSensor2ISR() {
  unsigned long now = micros();
  deltaTime2 = now - lastPulseTime2;
  lastPulseTime2 = now;
}

void setup() {
  Serial.begin(9600);
  pinMode(hallSensorPin1, INPUT_PULLUP);
  pinMode(hallSensorPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hallSensorPin1), hallSensor1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(hallSensorPin2), hallSensor2ISR, RISING);
}

void loop() {
  if (deltaTime1 > 0) {
    currentVelocity1 = calculateVelocity(deltaTime1);
    deltaTime1 = 0;
  }

  if (deltaTime2 > 0) {
    currentVelocity2 = calculateVelocity(deltaTime2);
    deltaTime2 = 0;
  }


  voltage = readVoltage();


  Serial.print("Velocity1:");
  Serial.print(currentVelocity1);
  Serial.print(",Velocity2:");
  Serial.print(currentVelocity2);
  Serial.print(",Voltage:");
  Serial.println(voltage);

  delay(1000); // Odczekaj 1 sekundę przed następnym pomiarem
}

float calculateVelocity(unsigned long deltaTime) {
  float radius = 457.2 / 2.0; // Promień koła w milimetrach
  float velocity_mm_per_sec = (2.0 * PI * radius) / (deltaTime / 1000000.0); // Oblicz prędkość w milimetrach na sekundę
  float velocity_km_per_hour = velocity_mm_per_sec * 3.6; // Konwersja na kilometry na godzinę
  return velocity_km_per_hour;
}

float readVoltage() {
  // Odczytaj napięcie z supercapów
  float voltage = analogRead(voltageSensorPin) * (5.0 / 1023.0); // Skonwertuj na napięcie w woltach
  return voltage;
}
