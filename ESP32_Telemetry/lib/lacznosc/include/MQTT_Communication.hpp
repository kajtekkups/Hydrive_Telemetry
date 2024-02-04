#ifndef MQTT_COMMUNICATION_HPP
#define MQTT_COMMUNICATION_HPP

#include <WiFiClientSecure.h>
#include <MQTTClient.h>   //AWS old
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "Pin_Defines.hpp"

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
void publish_MQTT_message(const char* topic, String payload);


/**** Method for initializing MQTT connection **********/
void establish_mqtt_connection();

#endif