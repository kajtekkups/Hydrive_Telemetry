#include "HallSensor.h"

HallSensor::HallSensor() {}

void HallSensor::addRotationSensor() {
    rotationsSensor1++;
    rotationsSensor2++;
}

void HallSensor::calculateVelocity1() {
    unsigned long currentTime = millis();

    if (lastVelocityMeasure1 + measureTime < currentTime) {
        float wheelDiameterMeters = WHEEL_DIAMETER / 1000.0;
        float wheelCircumference = wheelDiameterMeters * PI;
        currentVelocity1 = (rotationsSensor1 * wheelCircumference * 3.6) / (currentTime - lastVelocityMeasure1);
        lastVelocityMeasure1 = currentTime;
        rotationsSensor1 = 0;
    }
}

void HallSensor::calculateVelocity2() {
    unsigned long currentTime = millis();

    if (lastVelocityMeasure2 + measureTime < currentTime) {
        float wheelDiameterMeters = WHEEL_DIAMETER / 1000.0;
        float wheelCircumference = wheelDiameterMeters * PI;
        currentVelocity2 = (rotationsSensor2 * wheelCircumference * 3.6) / (currentTime - lastVelocityMeasure2);
        lastVelocityMeasure2 = currentTime;
        rotationsSensor2 = 0;
    }
}

void HallSensor::setup() {
    pinMode(VELOCITY_MEASURE_PIN_1, INPUT_PULLUP);
    pinMode(VELOCITY_MEASURE_PIN_2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_1), addRotationSensor, RISING);
    attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_2), addRotationSensor, RISING);

    rotationsSensor1 = 0;
    rotationsSensor2 = 0;
    lastVelocityMeasure1 = millis();
    lastVelocityMeasure2 = millis();
    measureTime = 1000;
}

void HallSensor::loop() {
    calculateVelocity1();
    calculateVelocity2();
    delay(10);
}
