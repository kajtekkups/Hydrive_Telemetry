#include "GNSS.hpp"
#include "Pin_Defines.hpp"

TinyGPSPlus gps;

void GPS_init(){
    gpsPort.begin(9600,  SERIAL_8N1, RX1, TX1);
    Serial.println("GPS established");
}


void GPS_update_data(){
    while (gpsPort.available() > 0)
        gps.encode(gpsPort.read());
}

void GPS_read_data(float& speed, float& latitude, float& longitude){
    double temp;

    if(gps.passedChecksum()){
        Serial.print("");
        if (gps.location.isUpdated() & gps.speed.isUpdated()){
            temp = gps.location.lat();
            latitude = (float)temp;

            temp = gps.location.lng();
            longitude = (float)temp;

            temp = gps.speed.kmph();
            speed = float(temp);
        }
    }
}