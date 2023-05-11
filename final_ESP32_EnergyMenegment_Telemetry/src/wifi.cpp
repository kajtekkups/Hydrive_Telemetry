// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <Wire.h>
// //#include <ArduinoJson.h>
// #include <vector>
// // #include "google_credentials.h"


// //Things to change
// const char * ssid = "Redmi 9";
// const char * password = "japierdole";

// const char* google_sheet_id = "1qhlmVOiLYXe3SzL2xRnl1asmH4zkSzdwy-nsB7Jt6kQ";
// const char* google_sheet_tab = "modul_pomiarowy";
// const char* google_api_key = "AKfycbzo6YQCcb9osI7DzXxUlpEdbzH5Dwk7a-qveLuW1Vd2TjcCy5ZSn_Y4TbP2a0YvRMJPdw";

// WiFiClientSecure client;

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
// }


// void sendDataToGoogleSheets(float value1, float value2) {
//   HTTPClient http;

//   String Values =  "&value1=" + String(value1) + "&value2=" + String(value2);
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



// //
// void loop() {
// //


//     // Odczytaj dane z czujników
//     float value1 = 44.0;
//     float value2 = 5.0;
//   sendDataToGoogleSheets(value1,value2);
//   delay(15);
// }




