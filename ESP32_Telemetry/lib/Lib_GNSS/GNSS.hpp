#ifndef GNSS
#define GNSS

#include "TinyGPS++.h"

#define gpsPort Serial1

extern TinyGPSPlus gps;

void GPS_init();

void GPS_update_data();

void GPS_read_data(float& speed, float& latitude, float& longitude);

#endif //GNSS