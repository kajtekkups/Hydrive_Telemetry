#include "DataManagement.hpp"


DataManagement data_management_instance;


void DataManagement::collectData(){

  measure_data.time = millis();

  int16_t voltage_value_vt;
  int16_t voltage_value_I;

  for(uint8_t i = 0; i < ADC_number; i++){    
    ads[i].getMessurements(voltage_value_vt, voltage_value_I);
    float temp = electrical_meassurements_instance.computeAdcConverterVolts(voltage_value_vt);
    float temp1 = electrical_meassurements_instance.computeAdcConverterVolts(voltage_value_I);
    measure_data.voltage_measurement[i] = electrical_meassurements_instance.calculateVolt(temp);
    measure_data.current_measurement[i] = electrical_meassurements_instance.calculateAmp(temp1);  
  }
  
  measure_data.velocity = hall_sensor_instance.current_velocity;

  measure_data.measurement_time = millis() - measure_data.time;

  position_instance.GpsReadData(measure_data.GPS_speed, measure_data.latitude, measure_data.longitude);
}


void DataManagement::saveSendData(){
  // data is send in json format 
  StaticJsonDocument<400> doc; 

  //prepare data for transmition
  for(uint8_t i; i < ADC_number; i++){
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

  Serial.println(package);
  //save data
  // micro_sd_file.appendFile(package);


  // //send data
  server_communication_instance.publishMqttMessage(package);
}
