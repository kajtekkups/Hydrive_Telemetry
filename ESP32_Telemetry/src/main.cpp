#include <Arduino.h>
#include "Adafruit_ADS1X15.h"
#include "pomiary_elektryczne.hpp"
#include "MQTT_Communication.hpp"

//leds variables
#define ADS_LED 5
#define WIFI_LED 10


//declere Analog Digital Converter module
// Adafruit_ADS1015 ads;
// Adafruit_ADS1015 ads1;

void setup() {
  
  //define ESP-PC wire communication
  Serial.begin(115200);
  Serial.println("PC Serial connection established");

  establish_mqtt_connection();


  // Activate default ads (addr floating or gnd)
  // ewentualnie  Adafruit_ADS1015 ads( 0x48 );
  // if (!ads.begin(0x48)) {
  //     pinMode(ADS_LED, OUTPUT);
  //     digitalWrite(ADS_LED, HIGH);
  //     while(1);
  // }

  //Activate ads1 (addr to sda)
  //   if (!ads1.begin(0x4a)) {
  //     pinMode(ADS_LED, OUTPUT);
  //     digitalWrite(ADS_LED, HIGH);
  //     while(1);
  // }
  // digitalWrite(ADS_LED, LOW);

}

void loop() {

  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected

  // //first ads
  //  int16_t results = ads.readADC_SingleEnded(0);
  // float pomiarVT_0 = CalculateVolt_for_meter(ads.computeVolts(results));
  
  // int16_t results1 = ads.readADC_SingleEnded(2);
  // float pomiarVT_1 = CalculateVolt_for_meter(ads.computeVolts(results1));
  
  // int16_t results2 = ads.readADC_SingleEnded(1);
  // float pomiarI_0 = CalculateAmp(results2);

  // int16_t results3 = ads.readADC_SingleEnded(3);
  // float pomiarI_1 = CalculateAmp(results3);

  //second ads
  // int16_t results4 = ads1.readADC_SingleEnded(1);
  // float pomiarV_2 = CalculateVolt_for_meter(ads.computeVolts(results4));
  // int16_t results5 = ads1.readADC_SingleEnded(2);
  // float pomiarI_2 = CalculateAmp(results5);
// float pomiarI_2 = 0;
// float pomiarV_2 = 0;

//   publishMessage(pomiarVT_0, pomiarVT_1, pomiarV_2, pomiarI_0, pomiarI_1, pomiarI_2);
//   client.loop();  //do wyjebania?

  DynamicJsonDocument doc(1024);

  doc["deviceId"] = "ESP32";
  doc["TEST"] = 1234;


  char mqtt_message[128];
  serializeJson(doc, mqtt_message);

  publish_message("esp32/pub", mqtt_message);
  delay(3000);
}