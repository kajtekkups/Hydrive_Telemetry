#include "DataManagement.hpp"


/**************************************************************************/
/*!
    @brief  Converts ADC reading into volts.

    @param counts the ADC reading in raw counts

    @return the ADC reading in volts
*/
/**************************************************************************/
float computeVolts(int16_t counts) {
  // see data sheet Table 3
  return ((float)counts) * (FS_RANGE / ADC_RESOLUTION);
}


void Collect_data(){

  measure_data.time = millis();

  int16_t voltage_value_vt;
  int16_t voltage_value_I;

  for(uint8_t i = 0; i < ADC_number; i++){    
    ads[i].get_messurements(voltage_value_vt, voltage_value_I);
    float temp = computeVolts(voltage_value_vt);
    float temp1 = computeVolts(voltage_value_I);
    measure_data.voltage_measurement[i] = CalculateVolt(temp);
    measure_data.current_measurement[i] = CalculateAmp(temp1);  
  }
  
  
  measure_data.velocity = hallSensorInstance.currentVelocity;

  measure_data.measurement_time = millis() - measure_data.time;
  
}


void Send_save_data(){
  // data is send in json format 
  StaticJsonDocument<400> doc; 

  //prepare data for transmition
  for(uint8_t i; i < ADC_number; i++){
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
