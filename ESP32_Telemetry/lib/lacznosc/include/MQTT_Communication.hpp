#ifndef MQTT_COMMUNICATION_HPP
#define MQTT_COMMUNICATION_HPP

#include <WiFiClientSecure.h>
#include <MQTTClient.h>   //AWS old
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"


/******* WIFI credentials *******/
#define WIFI_SSID "ESP32test"          
#define WIFI_PASSWORD "maslo123"

/******* MQTT Topic name *******/
#define MQTT_PUBLISH_TOPIC   "esp32/pub"

/******* MQTT Broker Connection Details *******/
#define mqtt_server "6aa9c1de47b0404e82629f1961517374.s1.eu.hivemq.cloud"
#define mqtt_username "dataServer"
#define mqtt_password "Hydrive1"
#define mqtt_port 8883


/**** Secure WiFi Connectivity Initialisation *****/
extern WiFiClientSecure espClient;


/**** MQTT Client Initialisation Using WiFi Connection *****/
extern PubSubClient MQTT_client;


/************* Connect to WiFi ***********/
void setup_wifi();

/************* Connect to MQTT Broker ***********/
void MQTT_reconnect();


/**** Method for Publishing MQTT Messages **********/
void publish_message(const char* topic, String payload);
// void publishMessage(float pomiarVT_0, float pomiarVT_1, float pomiarV_2, float pomiarI_0, float pomiarI_1, float pomiarI_2)
// {
//   StaticJsonDocument<200> doc;
//   doc["time"] = millis();
//   doc["pomiarVT_0"] = pomiarVT_0;
//   doc["pomiarVT_1"] = pomiarVT_1;
//   doc["pomiarV_2"] = pomiarV_2;
//   doc["pomiarI_0"] = pomiarI_0;
//   doc["pomiarI_1"] = pomiarI_1;
//   doc["pomiarI_2"] = pomiarI_2;

//   char jsonBuffer[256];
//   serializeJson(doc, jsonBuffer); // print to client

//   client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
// }


/**** Method for initializing MQTT connection **********/
void establish_mqtt_connection();

#endif