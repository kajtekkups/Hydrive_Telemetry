// WiFi conection
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
// Mesurment
#include <Arduino.h>
#include "../include/Adafruit_ADS1X15.h"
#include "../include/DallasTemperature/DallasTemperature.h"
#include "../include/OneWire/OneWire.h"
#include "../include/DallasTemperature/DallasTemperature.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

//leds variables
#define ADS_LED 5
#define WIFI_LED 10

// Data wire is connected to GPIO9
#define ONE_WIRE_BUS 9


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



float CalculateAmp(float Measure){
  float Amp; 

  /* rozdzielczosc modulu pomiaru pradu wynosi 100A ( maks 50A, ale jest mozliwosc pomiaru -50 - 50 )
  *  natomiast rozdzielczość ADC 2048, jednak nasze napiecie referencyjne to 5V a ADC mierzy 0-6,144V,
  *  dlatego realna rozdzielczosc to 1666,67, co mozna policzyc z prostej proporcji:
  * 
  *             6,144 = 2048
  *                5  =   x
  * 
  * Aby uzyskac poprawny wynik, zmierzone próbki [Measure] mnozymy przez wartosc jednej probki
  */
//   Amp =  (Measure) * (100 / 1666); 
  // Amp = VoltMeasure * 20;
 
  Amp = (Measure - 833.3) * 5/1666.67 / 0.04 - 0.65;
  return Amp; //amperomierz mierzy w zakresie -50 - 50 A, ale my liczymy wynik w zakresie 0 - 100
  }


float CalculateVolt_for_meter(float Measure){
  float Volt;

  Volt = Measure * 10;

  return Volt;
}


void publishMessage(float Temp, float pomiarVT_0, float pomiarVT_1, float pomiarV_2, float pomiarI_0, float pomiarI_1, float pomiarI_2)
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["Temp"] = Temp;
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


// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//temperature sensor
DeviceAddress sensor1 = { 0x28, 0xFF, 0x7D, 0xAC, 0x7C, 0x18, 0x1, 0xE2 };

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

  // start temperature sensors
  sensors.begin();
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



  
  
  sensors.requestTemperatures(); // Send the command to get temperatures

  float Tempreature = sensors.getTempC(sensor1);

  publishMessage(Tempreature, pomiarVT_0, pomiarVT_1, pomiarV_2, pomiarI_0, pomiarI_1, pomiarI_2);
  client.loop();  //do wyjebania?
  delay(300);
}