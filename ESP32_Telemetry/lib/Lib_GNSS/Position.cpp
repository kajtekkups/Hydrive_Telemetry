#include "Position.hpp"
#include "Pin_Defines.hpp"

Position position_instance;

void Position::begin(){
    gpsPort.begin(9600,  SERIAL_8N1, RX1, TX1);
}


void Position::GpsUpdatePosition(){
    while (gpsPort.available() > 0)
        gps.encode(gpsPort.read());
}

void Position::GpsReadData(float& speed, float& latitude, float& longitude){
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