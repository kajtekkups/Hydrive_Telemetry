// #include <Arduino.h>
// #include "../include/Adafruit_ADS1X15.h"
// #include "../include/DallasTemperature/DallasTemperature.h"

// #include <WiFi.h>
// #include <HTTPClient.h>

// #include "../include/OneWire/OneWire.h"
// #include "../include/DallasTemperature/DallasTemperature.h"


// //wifi credentials
// const char * ssid = "Redmi 9";
// const char * password = "123";

// // google sheets credentials
// const char* google_sheet_id = "1qhlmVOiLYXe3SzL2xRnl1asmH4zkSzdwy-nsB7Jt6kQ";
// const char* google_sheet_tab = "modul_pomiarowy";
// const char* google_api_key = "AKfycbyzLdWR3K0i9cBMGajPWXmUBMUBSd1Avw4kSKtYtpxKjKe_0bhq2AiyZ2QAe08cQ_tE9g";

// WiFiClientSecure client;

// //leds variables
// #define ADS_LED 5
// #define WIFI_LED 10

// // Data wire is connected to GPIO9
// #define ONE_WIRE_BUS 9
// // Setup a oneWire instance to communicate with a OneWire device
// OneWire oneWire(ONE_WIRE_BUS);
// // Pass our oneWire reference to Dallas Temperature sensor 
// DallasTemperature sensors(&oneWire);

// //temperature sensor
// DeviceAddress sensor1 = { 0x28, 0xFF, 0x7D, 0xAC, 0x7C, 0x18, 0x1, 0xE2 };

// //declere Analog Digital Converter module
// Adafruit_ADS1015 ads;
// Adafruit_ADS1015 ads1;


// //one wire temperture sensor 
// // OneWire oneWire(ONE_WIRE_BUS);

// float CalculateAmp(float Measure){
//   float Amp; 

//   /* rozdzielczosc modulu pomiaru pradu wynosi 100A ( maks 50A, ale jest mozliwosc pomiaru -50 - 50 )
//   *  natomiast rozdzielczość ADC 2048, jednak nasze napiecie referencyjne to 5V a ADC mierzy 0-6,144V,
//   *  dlatego realna rozdzielczosc to 1666,67, co mozna policzyc z prostej proporcji:
//   * 
//   *             6,144 = 2048
//   *                5  =   x
//   * 
//   * Aby uzyskac poprawny wynik, zmierzone próbki [Measure] mnozymy przez wartosc jednej probki
//   */
// //   Amp =  (Measure) * (100 / 1666); 
//   // Amp = VoltMeasure * 20;
 
//   Amp = (Measure - 833.3) * 5/1666.67 / 0.04 - 0.65;
//   return Amp; //amperomierz mierzy w zakresie -50 - 50 A, ale my liczymy wynik w zakresie 0 - 100
//   }

// float CalculateVolt_for_meter(float Measure){
//   float Volt;

//   Volt = Measure * 10;

//   return Volt;
// }


// void setup() {
  
//   //define ESP-PC wire communication
//   Serial.begin(115200);
//   Serial.println("Hello!");


//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//     pinMode(WIFI_LED, OUTPUT);
//     digitalWrite(WIFI_LED, HIGH);
//   }
//   digitalWrite(WIFI_LED, LOW);

//   //Activate default ads (addr floating or gnd)
//   //ewentualnie  Adafruit_ADS1015 ads( 0x48 );
//   if (!ads.begin(0x48)) {
//       pinMode(ADS_LED, OUTPUT);
//       digitalWrite(ADS_LED, HIGH);
//       while(1);
//   }

//   //Activate ads1 (addr to sda)
//     if (!ads1.begin(0x4a)) {
//       pinMode(ADS_LED, OUTPUT);
//       digitalWrite(ADS_LED, HIGH);
//       while(1);
//   }
//   digitalWrite(ADS_LED, LOW);

//   //start temperature sensors
//   sensors.begin();
// }
// // https://script.google.com/macros/s/AKfycbyNl8URkR5aVOybBJgbMbses3ZvavyRzfsSiXMnjYiQM39D_jyPFp7sZrhxAren1IdE9g/exec?sheet_id=1qhlmVOiLYXe3SzL2xRnl1asmH4zkSzdwy-nsB7Jt6kQ&tab=modul_pomiarowy&value1=1&value2=2&value3=5&value4=7&value5=7&value6=0&value7=8
// // https://script.google.com/macros/s/AKfycbwVAXC_PlinpxgsM8VNbtbY4ZbfL4mgvERmptS3JmyI/dev?sheet_id=1qhlmVOiLYXe3SzL2xRnl1asmH4zkSzdwy-nsB7Jt6kQ&tab=modul_pomiarowy&value1=1&value2=2&value3=5&value4=7&value5=7&value6=0&value7=8
// void sendDataToGoogleSheets(unsigned long CurrentTime, float Temp, float pomiarVT_0, float pomiarVT_1, float pomiarV_2, float pomiarI_0, float pomiarI_1, float pomiarI_2) {
//   HTTPClient http;

//   String Values =  "&value1=" + String(CurrentTime) + "&value2=" + String(Temp) + "&value3=" + String(pomiarVT_0) + \
//                    "&value4=" + String(pomiarVT_1) + "&value5=" + String(pomiarV_2) + "&value6=" + String(pomiarI_0) + \
//                    "&value7=" + String(pomiarI_1) + "&value8=" + String(pomiarI_2);
                   
//   http.begin("https://script.google.com/macros/s/" + String(google_api_key) + "/exec?sheet_id=" + String(google_sheet_id) + "&tab=" + String(google_sheet_tab) + Values);
//   int httpResponseCode = http.GET();

//   if (httpResponseCode > 0) {
//     String response = http.getString();
//     Serial.println("Data sent successfully");
//     Serial.println(response);
//   } else {
//     Serial.print("Error sending data. HTTP response code: ");
//     Serial.println(httpResponseCode);
//   }

//   http.end();
// }

// void loop() { 

//   //first ads
//    int16_t results = ads.readADC_SingleEnded(0);
//   float pomiarVT_0 = CalculateVolt_for_meter(ads.computeVolts(results));
  
//   int16_t results1 = ads.readADC_SingleEnded(2);
//   float pomiarVT_1 = CalculateVolt_for_meter(ads.computeVolts(results1));
  
//   int16_t results2 = ads.readADC_SingleEnded(1);
//   float pomiarI_0 = CalculateAmp(results2);

//   int16_t results3 = ads.readADC_SingleEnded(3);
//   float pomiarI_1 = CalculateAmp(results3);

//   //second ads
//   int16_t results4 = ads1.readADC_SingleEnded(1);
//   float pomiarV_2 = CalculateVolt_for_meter(ads.computeVolts(results4));
//   int16_t results5 = ads1.readADC_SingleEnded(2);
//   float pomiarI_2 = CalculateAmp(results5);


//   sensors.requestTemperatures(); // Send the command to get temperatures
//   unsigned long CurrentTime = millis();

//   float Tempreature = sensors.getTempC(sensor1);

//   sendDataToGoogleSheets(CurrentTime, Tempreature, pomiarVT_0, pomiarVT_1, pomiarV_2, pomiarI_0, pomiarI_1, pomiarI_2);
//   delay(15);

// }
