#include "HallSensor.h"

HallSensor hallSensorInstance;

volatile long HallSensor::rotationsSensor2 = 0;
unsigned long HallSensor::interruptTime = 0;

HallSensor::HallSensor() {}

void HallSensor::addRotationSensor() {

    if(millis() - interruptTime > 30){
        rotationsSensor2++;
        interruptTime = millis();
    }  
}

void HallSensor::calculateVelocity() {
    noInterrupts();

    unsigned long currentTime = millis();
    uint8_t velocityInKilometersPerHour;

    if (lastVelocityMeasure + measureTime < currentTime) {

        float distance = rotationsSensor2 * WHEEL_CIRCUMFERENCE;  //distance
        float local_time = currentTime - lastVelocityMeasure;
        local_time = local_time/1000;  //[s]
        float velocity = distance / local_time; //[m/s]
        velocity = velocity * 3.6; // [km/h]

        lastVelocityMeasure = currentTime;
        rotationsSensor2 = 0;

        currentVelocity = velocity;
    }

    interrupts();
}

void HallSensor::setup() {
    pinMode(VELOCITY_MEASURE_PIN_2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(VELOCITY_MEASURE_PIN_2), addRotationSensor, RISING);

    lastVelocityMeasure = millis();
    measureTime = 1500;
}

void HallSensor::loop() {
    calculateVelocity();
}
