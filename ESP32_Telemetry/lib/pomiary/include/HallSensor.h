#ifndef HallSensor_h
#define HallSensor_h

#include <Arduino.h>

#define VELOCITY_MEASURE_PIN_1 16
#define VELOCITY_MEASURE_PIN_2 4
const float WHEEL_CIRCUMFERENCE = 1.4444; // obwod kola w m (zmierzyc musze) (srednica 23cm okolo)

class HallSensor {
public:
    HallSensor();
    void setup();
    void loop();

    float currentVelocity;
    
private:
    static void addRotationSensor();
    void calculateVelocity();


    static volatile long rotationsSensor1;
    static volatile long rotationsSensor2;
    static unsigned long interruptTime;

    unsigned long lastVelocityMeasure;
    unsigned long measureTime;
};

extern HallSensor hallSensorInstance;

#endif
