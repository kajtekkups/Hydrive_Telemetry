#ifndef HallSensor_h
#define HallSensor_h

#include <Arduino.h>

#define VELOCITY_MEASURE_PIN_1 21
#define VELOCITY_MEASURE_PIN_2 22
#define WHEEL_DIAMETER 460.0 // srednica kola w mm (zmierzyc musze)

class HallSensor {
public:
    HallSensor();
    void setup();
    void loop();

    float currentVelocity;

private:
    static void addRotationSensor();
    void calculateVelocity();

    volatile long rotationsSensor1;
    volatile long rotationsSensor2;

    unsigned long lastVelocityMeasure;
    unsigned long measureTime;
};

#endif
