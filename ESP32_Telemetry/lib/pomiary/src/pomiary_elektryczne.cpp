#include "pomiary_elektryczne.hpp"


void AdsNodeInterface::begin(Adafruit_ADS1015* node_ads, uint8_t initialized, uint8_t current_pin, uint8_t voltage_pin){
  _node_ads = node_ads; 
  _initialized = initialized;
  _current_pin = current_pin;
  _voltage_pin = voltage_pin;
}


void AdsNodeInterface::get_messurements(int16_t& voltage, int16_t& current){
  if(_initialized){
    voltage = _node_ads->readADC_SingleEnded(_voltage_pin);
    current = _node_ads->readADC_SingleEnded(_current_pin);
  }
  else{
    voltage = 0;
    current = 0;
  }
}

// listy dostepnych przetwornikow ADS
Adafruit_ADS1015 ads[LICZBA_PRZETWORNIKOW];
AdsNodeInterface ads_nodes[LICZBA_PRZETWORNIKOW];

void init_ADC(){

  /******* aktywuj przetworniki ADC, jeżeli wystąpi problem, zasygnalizuj go diodą ******/
  pinMode(ADC_STATE_LED, OUTPUT);

  if (!ads[0].begin(ADS_ADRESS_ZA_OGNIWEM)) {
    Serial.println("blad polaczenia z ADS 0");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_nodes[0].begin(&ads[0], 0, ADS_0_CURRENT, ADS_0_VOLT);
  }
  else{
    ads_nodes[0].begin(&ads[0], 1, ADS_0_CURRENT, ADS_0_VOLT);
  }
  
  if (!ads[1].begin(ADS_ADRESS_PRZED_PRZETWORNICAMI)) {
    Serial.println("blad polaczenia z ADS 1");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_nodes[1].begin(&ads[1], 0, ADS_1_CURRENT, ADS_1_VOLT);
  }
  else{
    ads_nodes[1].begin(&ads[1], 1, ADS_1_CURRENT, ADS_1_VOLT);
  }

  if (!ads[2].begin(ADS_ADRESS_ZA_PRZETWORNICAMI)) {
    Serial.println("blad polaczenia z ADS 2");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_nodes[2].begin(&ads[2], 0, ADS_2_CURRENT, ADS_2_VOLT);
  }
  else{
    ads_nodes[2].begin(&ads[2], 1, ADS_2_CURRENT, ADS_2_VOLT);
  }

  if(!ads[3].begin(ADS_ADRESS_KONDENSATORY)) {
    Serial.println("blad polaczenia z ADS 3");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_nodes[3].begin(&ads[3], 0, ADS_3_CURRENT, ADS_3_VOLT);
  }
  else{
    ads_nodes[3].begin(&ads[3], 1, ADS_3_CURRENT, ADS_3_VOLT);
  }
}


float CalculateAmp(float Measure_VT){
  
  //amperomierz mierzy w zakresie -50 --- 50 A
  // 0A odpowiada połowie napięcia zasilającego przetwornik ADC (dla 5V będzie to 2.5V)

  float pomiar_znormalizowany = Measure_VT - (NAPIECIE_REFERENCYJNE/2); 

  float Amp = pomiar_znormalizowany / ACS758_SENSITIVITY_DEFAULT - BLAD_POMIARU;
  
  return Amp; 
  }


float CalculateVolt(float Measure){
  float Volt;

  Volt = Measure * 10;

  return Volt;
}


void Collect_electrical_data(){

  int16_t results_vt;
  int16_t results_I;

  for(uint8_t i = 0; i < LICZBA_PRZETWORNIKOW; i++){
    
    ads_nodes[i].get_messurements(results_vt, results_I);
    
    dane_elektryczne.pomiar_VT[i] = CalculateVolt(ads[i].computeVolts(results_vt));
    dane_elektryczne.pomiar_I[i] = CalculateAmp(ads[i].computeVolts(results_I));  
  }
  
  dane_elektryczne.time = millis();
}


void Send_save_electrical_data(){
  StaticJsonDocument<400> doc;  // data is send in json format 

  // pobierz dane do zapisu
  for(uint8_t i; i < LICZBA_PRZETWORNIKOW; i++){
    doc["pomiar_VT" + std::to_string(i)] = dane_elektryczne.pomiar_VT[i]; //zapisz napiecie
    doc["pomiar_I" + std::to_string(i)] = dane_elektryczne.pomiar_I[i]; //zapisz natezenie
  }
  doc["time_ms"] = dane_elektryczne.time;

  //konwertuj dane
  char mqtt_message[400];
  serializeJson(doc, mqtt_message);

  //wyslij na serwer
  publish_MQTT_message(MQTT_PUBLISH_TOPIC, mqtt_message);
}