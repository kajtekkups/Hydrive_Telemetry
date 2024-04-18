#include "HallSensor.h"

HallSensor hallSensorInstance;

volatile long HallSensor::rotationsSensor1 = 0;
volatile long HallSensor::rotationsSensor2 = 0;
unsigned long HallSensor::interruptTime = 0;

HallSensor::HallSensor() {}

void HallSensor::addRotationSensor() {
    if(millis() - interruptTime > 100){
        rotationsSensor1++;
        rotationsSensor2++;
        interruptTime = millis();
    }    
}

void HallSensor::calculateVelocity() {
    unsigned long currentTime = millis();
    uint8_t velocityInKilometersPerHour;

    if (lastVelocityMeasure + measureTime < currentTime) {
        // float velocity1Local = (rotationsSensor1 * WHEEL_CIRCUMFERENCE) / (currentTime - lastVelocityMeasure);
        float velocity2Local = (rotationsSensor2 * WHEEL_CIRCUMFERENCE) / ((currentTime - lastVelocityMeasure)/1000);  //[m/s]
        velocityInKilometersPerHour = uint8_t(velocity2Local * 3.6);

        lastVelocityMeasure = currentTime;
        // rotationsSensor1 = 0;
        rotationsSensor2 = 0;

        currentVelocity = velocityInKilometersPerHour;
        // if (fabs(velocity1Local) > 0.01 && fabs(velocity2Local) > 0.01) {
        //     float averageVelocity = (velocity1Local + velocity2Local) / 2;
        //     currentVelocity = averageVelocity;
        // } else if (fabs(velocity1Local) > 0.01) {
        //     currentVelocity = velocity1Local;
        // } else if (fabs(velocity2Local) > 0.01) {
        //     currentVelocity = velocity2Local;
        // }
    }
}

void HallSensor::setup() {
    // pinMode(VELOCITY_MEASURE_PIN_1, INPUT_PULLUP);
    pinMode(VELOCITY_MEASURE_PIN_2, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_1), addRotationSensor, RISING);
    attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_2), addRotationSensor, RISING);

    lastVelocityMeasure = 0;
    measureTime = 500;
}

void HallSensor::loop() {
    calculateVelocity();
    // wyświetlanie na ekranie bolidu - miejsce na kod
    delay(10);
}
