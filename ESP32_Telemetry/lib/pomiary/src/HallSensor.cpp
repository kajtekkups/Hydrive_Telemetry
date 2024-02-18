#include "HallSensor.h"

HallSensor::HallSensor() {}

void HallSensor::addRotationSensor() {
    rotationsSensor1++;
}

void HallSensor::calculateVelocity() {
    unsigned long currentTime = millis();

    if (lastVelocityMeasure + measureTime < currentTime) {
        float velocity1Local = (rotationsSensor1 * WHEEL_DIAMETER) / (currentTime - lastVelocityMeasure);
        float velocity2Local = (rotationsSensor2 * WHEEL_DIAMETER) / (currentTime - lastVelocityMeasure);
        lastVelocityMeasure = currentTime;
        rotationsSensor1 = 0;
        rotationsSensor2 = 0;

        if (fabs(velocity1Local) > 0.01 && fabs(velocity2Local) > 0.01) {
            float averageVelocity = (velocity1Local + velocity2Local) / 2;
            currentVelocity = averageVelocity;
        } else if (fabs(velocity1Local) > 0.01) {
            currentVelocity = velocity1Local;
        } else if (fabs(velocity2Local) > 0.01) {
            currentVelocity = velocity2Local;
        }
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
    // wyświetlanie na ekranie bolidu
    delay(10);
}
