#include "pomiary_elektryczne.hpp"

Adafruit_ADS1015 ads[LICZBA_PRZETWORNIKOW];


void init_ADC(){

  /******* aktywuj przetworniki ADC, jeżeli wystąpi problem, zasygnalizuj go diodą ******/
  pinMode(ADC_LED_STATE, OUTPUT);

  if (!ads[0].begin(ADS_ADRESS_ZA_OGNIWEM)) {
    Serial.println("blad polaczenia z ADS 0");
    digitalWrite(ADC_LED_STATE, HIGH);
  }
  
  if (!ads[1].begin(ADS_ADRESS_PRZED_PRZETWORNICAMI)) {
    Serial.println("blad polaczenia z ADS 1");
    digitalWrite(ADC_LED_STATE, HIGH);
  }

  // if (!ads[2].begin(ADS_ADRESS_ZA_PRZETWORNICAMI)) {
  //   Serial.println("blad polaczenia z ADS 2");
  //   digitalWrite(ADC_LED_STATE, HIGH);
  // }

  // if(!ads[3].begin(ADS_ADRESS_KONDENSATORY)) {
  //   Serial.println("blad polaczenia z ADS 3");
  //   digitalWrite(ADC_LED_STATE, HIGH);
  // }
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
/* Docelowo stworzylbym klase, ktorej obiekty inicjalizowaloby sie w initADC
*  klasa miala by pole Adafruit_ADS1015 ads; przy inicjalizacji podawalibysmy 
*  Piny pomiarowe, a dodatkowo mialaby uniwersalna metode odczytujaca wartosci
*  z pinow. Ale to w przyszlosci
*/
  int16_t results_vt;
  int16_t results_I;

  //first ads
  results_vt = ads[0].readADC_SingleEnded(ADS_0_VOLT);
  results_I = ads[0].readADC_SingleEnded(ADS_0_CURRENT);
  dane_elektryczne.pomiar_VT[0] = CalculateVolt_for_meter(ads[0].computeVolts(results_vt));
  dane_elektryczne.pomiar_I[0] = CalculateAmp(results_I);

  //second ads
  results_vt = ads[1].readADC_SingleEnded(ADS_1_VOLT);  
  results_I = ads[1].readADC_SingleEnded(ADS_1_CURRENT);
  dane_elektryczne.pomiar_VT[1] = CalculateVolt_for_meter(ads[1].computeVolts(results_vt));
  dane_elektryczne.pomiar_I[1] = CalculateAmp(results_I);

  //third ads
  results_vt = ads[2].readADC_SingleEnded(ADS_2_VOLT);
  results_I = ads[2].readADC_SingleEnded(ADS_2_CURRENT);
  dane_elektryczne.pomiar_VT[2] = CalculateVolt_for_meter(ads[2].computeVolts(results_vt));
  dane_elektryczne.pomiar_I[2] = CalculateAmp(results_I);

  //fourth ads
  results_vt = ads[3].readADC_SingleEnded(ADS_3_VOLT);
  results_I = ads[3].readADC_SingleEnded(ADS_3_CURRENT);
  dane_elektryczne.pomiar_VT[3] = CalculateVolt_for_meter(ads[3].computeVolts(results_vt));
  dane_elektryczne.pomiar_I[3] = CalculateAmp(results_I);
  
  dane_elektryczne.time = millis();
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