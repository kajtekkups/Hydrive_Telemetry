#include "HallSensor.h"

HallSensor::HallSensor() {}

void HallSensor::addRotationSensor() {
    rotationsSensor1++;
    rotationsSensor2++;
}

void HallSensor::calculateVelocity() {
    unsigned long currentTime = millis();

    if (lastVelocityMeasure + measureTime < currentTime) {
        // Przeliczenie średnicy koła z mm na metry
        float wheelDiameterMeters = WHEEL_DIAMETER / 1000.0;

        // Obliczenie obwodu koła w metrach
        float wheelCircumference = wheelDiameterMeters * PI;

        // Obliczenie prędkości w metrach na sekundę dla każdego czujnika
        float velocity1Local = (rotationsSensor1 * wheelCircumference) / (currentTime - lastVelocityMeasure);
        float velocity2Local = (rotationsSensor2 * wheelCircumference) / (currentTime - lastVelocityMeasure);

        lastVelocityMeasure = currentTime;
        rotationsSensor1 = 0;
        rotationsSensor2 = 0;

        // Obliczenie średniej prędkości
        float averageVelocity = (velocity1Local + velocity2Local) / 2;

        // Przeliczenie prędkości z m/s na km/h
        currentVelocity = averageVelocity * 3.6; // 1 m/s = 3.6 km/h
    }
}

void HallSensor::setup() {
    pinMode(VELOCITY_MEASURE_PIN_1, INPUT_PULLUP);
    pinMode(VELOCITY_MEASURE_PIN_2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_1), addRotationSensor, RISING);
    attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_2), addRotationSensor, RISING);

    rotationsSensor1 = 0;
    rotationsSensor2 = 0;
    lastVelocityMeasure = 0;
    measureTime = 1000;
}

void HallSensor::loop() {
    calculateVelocity();
    delay(10);
}
