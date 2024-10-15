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

  doc["czas_pomiaru"] = measure_data.measurement_time;

  doc["time_ms"] = measure_data.time;

  //convert data
  char package[400];
  serializeJson(doc, package);

  Serial.println(package);
  //save data
  // micro_sd_file.appendFile(package);


  // //send data
  // publish_MQTT_message(MQTT_PUBLISH_TOPIC, package);
}
