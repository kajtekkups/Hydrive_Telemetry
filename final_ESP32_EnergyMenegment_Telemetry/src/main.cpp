// #include <Arduino.h>
// #include <BluetoothSerial.h>
// #include "../include/Adafruit_ADS1X15.h"
// #include "../include/DallasTemperature/DallasTemperature.h"
// // #include "../include/OneWire/OneWire.h"


// //leds variables
// #define ADS_LED 5
// #define BLUETOOTH_CONNECTED_LED 10

// //voltage types
// #define VOLTAGE_33 0
// #define VOLTAGE_5 1
// #define VOLTAGE_33_BATTERY 2

// // one wire is connected to GPIO15
// // #define ONE_WIRE_BUS 27

// //declere ADC module
// Adafruit_ADS1015 ads;
// Adafruit_ADS1015 ads1;

// // define ESP-PC wireless communication
// BluetoothSerial SerialBT;

// //one wire temperture sensor 
// // OneWire oneWire(ONE_WIRE_BUS);

// float CalculateAmp(float VoltMeasure){
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
// //   Amp =  (Measure) * (100 / 1666.67); 
//   Amp = VoltMeasure * 20.0;
 
//   return Amp - 50.0; //amperomierz mierzy w zakresie -50 - 50 A, ale my liczymy wynik w zakresie 0 - 100
//   }

// float CalculateVolt_for_meter(float Measure, int VTsupply){
//   float Volt;
//   if(VTsupply == VOLTAGE_5){
//     Volt = Measure * 10;
//   }
//   return Volt;
// }

// void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){

//   if (event == ESP_SPP_SRV_OPEN_EVT) {
//     digitalWrite (BLUETOOTH_CONNECTED_LED, LOW);
//   }
//   else if (event == ESP_SPP_CLOSE_EVT ) {
//     digitalWrite (BLUETOOTH_CONNECTED_LED, HIGH);
//   }
// }


// void setup() {
  
//   //define ESP-PC wire communication
//   Serial.begin(115200);
//   Serial.println("Hello!");


//   SerialBT.register_callback(callback);


//   SerialBT.begin("ESP32test");
//   delay(200);


//   pinMode(BLUETOOTH_CONNECTED_LED, OUTPUT);
//   digitalWrite(BLUETOOTH_CONNECTED_LED, HIGH);
  

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
// }

// void loop() { 

//   unsigned long CurrentTime = millis();
//   SerialBT.print("czas: "); SerialBT.print(CurrentTime); SerialBT.print(" [s], ");

//   //first ads
//    int16_t results = ads.readADC_SingleEnded(0);
//   SerialBT.print("pomiarVT_0: ");  SerialBT.print(CalculateVolt_for_meter(ads.computeVolts(results) , VOLTAGE_5)); SerialBT.print(" [V], ");
  
//   int16_t results1 = ads.readADC_SingleEnded(2);
//   SerialBT.print("pomiarVT_1: ");  SerialBT.print(CalculateVolt_for_meter(ads.computeVolts(results1) , VOLTAGE_5)); SerialBT.print(" [V], ");
  
//   int16_t results2 = ads.readADC_SingleEnded(1);
//   SerialBT.print("pomiarI_0: ");  SerialBT.print(CalculateAmp(ads.computeVolts(results2))); SerialBT.print(" [A], ");

//   int16_t results3 = ads.readADC_SingleEnded(3);
//   SerialBT.print("pomiarI_1: ");  SerialBT.print(CalculateAmp(ads.computeVolts(results3)));  SerialBT.print(" [A], ");

//   //second ads
//   int16_t results4 = ads1.readADC_SingleEnded(1);
//   SerialBT.print("pomiarV_2: ");  SerialBT.print(ads.computeVolts(results4));  SerialBT.print(" [V], ");
//   int16_t results5 = ads1.readADC_SingleEnded(2);
//   SerialBT.print("pomiarI_2: ");  SerialBT.print(CalculateAmp(ads.computeVolts(results5)));  SerialBT.println(" [A], ");


//   delay(120);

// }

//       // SerialBT.print(results); SerialBT.print(" ("); SerialBT.print(ads.computeVolts(results)); SerialBT.print("V) ");
//   // SerialBT.print(CalculateAmp(results, VOLTAGE_33)); SerialBT.println(" A");
//     // uint32_t voltage = esp_adc_cal_raw_to_voltage(reading, adc_chars);
//     // uint32_t voltage = esp_adc_cal_raw_to_voltage(reading, adc_chars);