#include "pomiary_elektryczne.hpp"


void init_ADC(){

    // Activate default ads (addr floating or gnd)
  // ewentualnie  Adafruit_ADS1015 ads( 0x48 );
  
  // if (!ads.begin(0x48)) {
  //   pinMode(ADS_LED, OUTPUT);
  //   digitalWrite(ADS_LED, HIGH);
  //   while(1);
  // }

  // Activate ads1 (addr to sda)
  
  // if (!ads1.begin(0x4a)) {
  //     pinMode(ADS_LED, OUTPUT);
  //     digitalWrite(ADS_LED, HIGH);
  //     while(1);
  // }
  // digitalWrite(ADS_LED, LOW);
}


float CalculateAmp(float Measure){
  float Amp; 
  
  //amperomierz mierzy w zakresie -50 - 50 A, ale my liczymy wynik w zakresie 0 - 100
  float pomiar_znormalizowany = Measure - (ROZDZIELCZOSC_PRZETWORNIKA_ADC/2);  // czujnik mierzy w zakresie -50 - 50 (rozdzielczosc 1666,6)
  float rozdzielczosc_bitu = NAPIECIE_REFERENCYJNE/ROZDZIELCZOSC_PRZETWORNIKA_ADC;  // rozdzielczosc JEDNEGO bitu przetwornika ADC 

  Amp = (pomiar_znormalizowany * rozdzielczosc_bitu) / ACS758_SENSITIVITY_DEFAULT - BLAD_POMIARU;
  
  return Amp; 
  }


float CalculateVolt_for_meter(float Measure){
  float Volt;

  Volt = Measure * 10;

  return Volt;
}


void Collect_electrical_data(){
  for(uint8_t i; i < LICZBA_POMIAROW; i++){
      dane_elektryczne.pomiar_VT[i] = (float)i;
  }
}


void Send_electrical_data(){
  StaticJsonDocument<300> doc;  // data is send in json format 

  // pobierz dane do zapisu
  for(uint8_t i; i < LICZBA_POMIAROW; i++){
    doc["pomiar_VT" + std::to_string(i)] = dane_elektryczne.pomiar_VT[i]; //zapisz napiecie
    doc["pomiar_I" + std::to_string(i)] = dane_elektryczne.pomiar_I[i]; //zapisz natezenie
  }
  doc["time_ms"] = dane_elektryczne.time;

  //konwertuj dane
  char mqtt_message[200];
  serializeJson(doc, mqtt_message);

  //wyslij na serwer
  publish_MQTT_message(MQTT_PUBLISH_TOPIC, mqtt_message);

}