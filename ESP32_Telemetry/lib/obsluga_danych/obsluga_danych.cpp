#include "obsluga_danych.hpp"


void Collect_data(){

  dane_pomiarowe.time = millis();

  // zbierz dane elektryczne
  int16_t results_vt;
  int16_t results_I;

  for(uint8_t i = 0; i < LICZBA_PRZETWORNIKOW; i++){    
    ads_objects[i].get_messurements(results_vt, results_I);
    
    dane_pomiarowe.pomiar_VT[i] = CalculateVolt(ads[i].computeVolts(results_vt));
    dane_pomiarowe.pomiar_I[i] = CalculateAmp(ads[i].computeVolts(results_I));  
  }
  
  // pobierz predkosc 
  dane_pomiarowe.velocity = hallSensorInstance.currentVelocity;

  // sprawdz czas pomiaru
  dane_pomiarowe.czas_pomiaru = millis() - dane_pomiarowe.time;
  
}


void Send_save_data(){
  // data is send in json format 
  StaticJsonDocument<400> doc; 

  // pobierz dane elektryczne do zapisu
  for(uint8_t i; i < LICZBA_PRZETWORNIKOW; i++){
    doc["pomiar_VT" + std::to_string(i)] = dane_pomiarowe.pomiar_VT[i]; //zapisz napiecie
    doc["pomiar_I" + std::to_string(i)] = dane_pomiarowe.pomiar_I[i]; //zapisz natezenie
  }
  
  // zapisz predkosc
  doc["predkosc"] = dane_pomiarowe.velocity;

  // zapisz opoznienie
  doc["czas_pomiaru"] = dane_pomiarowe.czas_pomiaru;

  // zapisz czas
  doc["time_ms"] = dane_pomiarowe.time;


  //konwertuj dane
  char package[400];
  serializeJson(doc, package);


  // zapisz na karcie
  micro_sd_file.appendFile(package);


  //wyslij na serwer
  publish_MQTT_message(MQTT_PUBLISH_TOPIC, package);
}
