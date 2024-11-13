#include "DataManagement.hpp"


void Collect_data(){

  measure_data.time = millis();

  int16_t results_vt;
  int16_t results_I;

  for(uint8_t i = 0; i < ADC_NUMBER; i++){    
    ads_objects[i].get_messurements(results_vt, results_I);
    
    measure_data.voltage_measurement[i] = CalculateVolt(ads[i].computeVolts(results_vt));
    measure_data.current_measurement[i] = CalculateAmp(ads[i].computeVolts(results_I));  
  }
  
  measure_data.velocity = hallSensorInstance.currentVelocity;

  measure_data.measurement_time = millis() - measure_data.time;

  GPS_read_data(measure_data.GPS_speed, measure_data.latitude, measure_data.longitude);
}


void Send_save_data(){
  // data is send in json format 
  StaticJsonDocument<400> doc; 

  //prepare data for transmition
  for(uint8_t i; i < ADC_NUMBER; i++){
    doc["pomiar_VT" + std::to_string(i)] = measure_data.voltage_measurement[i]; 
    doc["pomiar_I" + std::to_string(i)] = measure_data.current_measurement[i];
  }

  doc["predkosc"] = measure_data.velocity;

  doc["GPS_speed"] = measure_data.GPS_speed;

  String temp_high_precision_latitude = String(measure_data.latitude, 10);
  doc["latitude"] = temp_high_precision_latitude;
  String temp_high_precision_longitude = String(measure_data.longitude, 10);
  doc["longitude"] =  temp_high_precision_longitude;

  doc["czas_pomiaru"] = measure_data.measurement_time;

  doc["time_ms"] = measure_data.time;


  //convert data
  char package[400];
  serializeJson(doc, package);

  Serial.print("");
  Serial.println(package);
  //save data
  micro_sd_file.appendFile(package);


  //send data
  publish_MQTT_message(MQTT_PUBLISH_TOPIC, package);
}
