#ifndef HallSensor_h
#define HallSensor_h

#include <Arduino.h>

#define VELOCITY_MEASURE_PIN_1 21
#define VELOCITY_MEASURE_PIN_2 22
#define WHEEL_DIAMETER 457.2 //18 cali

class HallSensor {
public:
    HallSensor();
    void setup();
    void loop();

    float currentVelocity1;
    float currentVelocity2;


private:
    static void addRotationSensor();
    void calculateVelocity1();
    void calculateVelocity2();


    volatile long rotationsSensor1;
    volatile long rotationsSensor2;

    unsigned long lastVelocityMeasure1;
    unsigned long lastVelocityMeasure2;
    unsigned long measureTime;
};

#endif
