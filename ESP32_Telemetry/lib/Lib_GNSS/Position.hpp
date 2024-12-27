#ifndef GNSS
#define GNSS

#include "TinyGPS++.h"

#define gpsPort Serial1


class Position{
public:
    void begin();
    void GpsUpdatePosition();
    void GpsReadData(float& speed, float& latitude, float& longitude);

private:
    TinyGPSPlus gps;
};

extern Position position_instance;

#endif //GNSS