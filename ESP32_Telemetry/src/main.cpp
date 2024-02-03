#include <Arduino.h>
#include "Adafruit_ADS1X15.h"
#include "pomiary_elektryczne.hpp"
#include "MQTT_Communication.hpp"

//leds variables
#define ADS_LED 5
#define WIFI_LED 10


//define WiFi/cloud connection
WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
    pinMode(WIFI_LED, OUTPUT);
    digitalWrite(WIFI_LED, HIGH);
  }
  digitalWrite(WIFI_LED, LOW);

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);


  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  Serial.println("AWS IoT Connected!");
}


void publishMessage(float pomiarVT_0, float pomiarVT_1, float pomiarV_2, float pomiarI_0, float pomiarI_1, float pomiarI_2)
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["pomiarVT_0"] = pomiarVT_0;
  doc["pomiarVT_1"] = pomiarVT_1;
  doc["pomiarV_2"] = pomiarV_2;
  doc["pomiarI_0"] = pomiarI_0;
  doc["pomiarI_1"] = pomiarI_1;
  doc["pomiarI_2"] = pomiarI_2;

  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}



//declere Analog Digital Converter module
Adafruit_ADS1015 ads;
Adafruit_ADS1015 ads1;

void setup() {
  
  //define ESP-PC wire communication
  Serial.begin(115200);
  Serial.println("Hello!");

  connectAWS();

  // Activate default ads (addr floating or gnd)
  // ewentualnie  Adafruit_ADS1015 ads( 0x48 );
  if (!ads.begin(0x48)) {
      pinMode(ADS_LED, OUTPUT);
      digitalWrite(ADS_LED, HIGH);
      while(1);
  }

  //Activate ads1 (addr to sda)
  //   if (!ads1.begin(0x4a)) {
  //     pinMode(ADS_LED, OUTPUT);
  //     digitalWrite(ADS_LED, HIGH);
  //     while(1);
  // }
  digitalWrite(ADS_LED, LOW);

}

void loop() {

  //first ads
   int16_t results = ads.readADC_SingleEnded(0);
  float pomiarVT_0 = CalculateVolt_for_meter(ads.computeVolts(results));
  
  int16_t results1 = ads.readADC_SingleEnded(2);
  float pomiarVT_1 = CalculateVolt_for_meter(ads.computeVolts(results1));
  
  int16_t results2 = ads.readADC_SingleEnded(1);
  float pomiarI_0 = CalculateAmp(results2);

  int16_t results3 = ads.readADC_SingleEnded(3);
  float pomiarI_1 = CalculateAmp(results3);

  //second ads
  // int16_t results4 = ads1.readADC_SingleEnded(1);
  // float pomiarV_2 = CalculateVolt_for_meter(ads.computeVolts(results4));
  // int16_t results5 = ads1.readADC_SingleEnded(2);
  // float pomiarI_2 = CalculateAmp(results5);
float pomiarI_2 = 0;
float pomiarV_2 = 0;

  publishMessage(pomiarVT_0, pomiarVT_1, pomiarV_2, pomiarI_0, pomiarI_1, pomiarI_2);
  client.loop();  //do wyjebania?
  delay(300);
}