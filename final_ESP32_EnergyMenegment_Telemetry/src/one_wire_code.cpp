// /*
//  * Rui Santos 
//  * Complete Project Details https://randomnerdtutorials.com
//  */

// // Include the libraries we need
// #include "../include/OneWire/OneWire.h"
// #include "../include/DallasTemperature/DallasTemperature.h"


// // Data wire is connected to GPIO15
// #define ONE_WIRE_BUS 9
// // Setup a oneWire instance to communicate with a OneWire device
// OneWire oneWire(ONE_WIRE_BUS);
// // Pass our oneWire reference to Dallas Temperature sensor 
// DallasTemperature sensors(&oneWire);

// DeviceAddress sensor1 = { 0x28, 0xFF, 0x7D, 0xAC, 0x7C, 0x18, 0x1, 0xE2 };
// // DeviceAddress sensor2 = { 0x28, 0xFF, 0xCB, 0xAC, 0x7C, 0x18, 0x1, 0x31};
// // DeviceAddress sensor3= { 0x28, 0xFF, 0xA0, 0x11, 0x33, 0x17, 0x3, 0x96 };

// void setup(void){
//   Serial.begin(115200);
//   sensors.begin();
// }

// void loop(void){ 
//   Serial.print("Requesting temperatures...");
//   sensors.requestTemperatures(); // Send the command to get temperatures
//   Serial.println("DONE");
  
//   Serial.print("Sensor 1(*C): ");
//   Serial.print(sensors.getTempC(sensor1)); 
  
// //   Serial.print("Sensor 2(*C): ");
// //   Serial.print(sensors.getTempC(sensor2)); 
//   delay(2000);
// }