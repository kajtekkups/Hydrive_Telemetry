#include "EnergyManagement.hpp"

EnergyManagement energy_management_instance;

EnergyManagement::EnergyManagement(){
    memcpy(acceleration_point, accelerationPoints[acceleration_point_index], sizeof(accelerationPoints[acceleration_point_index]));
    memcpy(control_point, controlPoints[control_point_index], sizeof(controlPoints[control_point_index]));


    ledcSetup(mosfet_pwm_channel, pwmFreq, pwmResolution);
    ledcAttachPin(CAPACITOR_MOSFET_PWM, mosfet_pwm_channel);
}


void EnergyManagement::loop(){
    updatePosition();
    // // check if caps are full, if yes, update pwm


    if(checkIfInAccelerationPoint()){
        updatePwm(1);
        // requestAcceleration();
        
        measure_data.in_acceleration_point = 1;

        acceleration_point_index++;
        if(acceleration_point_index == acceleration_points_num){
            acceleration_point_index = 0;
        }
        memcpy(acceleration_point, accelerationPoints[acceleration_point_index], sizeof(accelerationPoints[acceleration_point_index]));
        
        for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
            last_acceleration_distances[i] = MAX_DISTANCE;
        }

    }else if(checkIfInControlPoint()){
        measure_data.in_control_point = 1;
        control_point_index++;
        if(control_point_index == control_points_num){
            control_point_index = 0;
        }
        memcpy(control_point, controlPoints[control_point_index], sizeof(controlPoints[control_point_index]));

        for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
            last_control_distances[i] = MAX_DISTANCE;
        }
        updatePwm();
    }else{
        measure_data.in_acceleration_point = 0;
        measure_data.in_control_point = 0;
    }
    Serial.println(acceleration_point_index);
}


void EnergyManagement::updatePwm(uint16_t acceleration_point){
    if (acceleration_point){
        ledcWrite(mosfet_pwm_channel, 125);
    } else{
    uint16_t driving_time = drivingTimeToNextPoint();
    uint16_t required_energy = estimateOverallCapRequiredEnergy();

    uint16_t pwm_width = calculatePwmWidth(driving_time, required_energy);
    sendPwmWidth(pwm_width);
    }
}


void EnergyManagement::updatePosition(){
    current_position[LATITUDE_INDEX] = measure_data.latitude;
    current_position[LONGITUDE_INDEX] = measure_data.longitude;
    
    for(uint16_t i = 0; i < LAST_POINTS_NUM - 1; ++i){
        last_acceleration_distances[i] = last_acceleration_distances[i+1];
        last_control_distances[i] = last_control_distances[i+1];
    }

    last_acceleration_distances[LAST_POINTS_NUM-1] = gps.distanceBetween(double(current_position[LATITUDE_INDEX]), double(current_position[LONGITUDE_INDEX]), double(acceleration_point[LATITUDE_INDEX]), double(acceleration_point[LONGITUDE_INDEX]));
    last_control_distances[LAST_POINTS_NUM-1] = gps.distanceBetween(double(current_position[LATITUDE_INDEX]), double(current_position[LONGITUDE_INDEX]), double(control_point[LATITUDE_INDEX]), double(control_point[LONGITUDE_INDEX]));
}


uint16_t EnergyManagement::drivingTimeToNextPoint(){
    return 0;
}


uint16_t EnergyManagement::estimateOverallCapRequiredEnergy(){
    return 0;
}


uint16_t EnergyManagement::calculatePwmWidth(uint16_t driving_time, uint16_t required_energy ){
// if caps are full or near full, set to 0
    return 0;
}


void EnergyManagement::sendPwmWidth(uint16_t pwm_width ){

}


bool EnergyManagement::checkIfInAccelerationPoint(){
    float mean_distance = 0.0;
    for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
        mean_distance += last_acceleration_distances[i];
    }
    mean_distance /= float(LAST_POINTS_NUM);
    
    Serial.println();
    Serial.println(mean_distance);

    if(mean_distance < REQUIRED_DISTANCE){
        return true;
    }
    return false;




    // return true;
}

bool EnergyManagement::checkIfInControlPoint(){
    float mean_distance = 0.0;
    for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
        mean_distance += float(last_control_distances[i]);
    }
    mean_distance /= float(LAST_POINTS_NUM);
    
    Serial.println("control point");
    Serial.println(mean_distance);

    if(mean_distance < REQUIRED_DISTANCE){
        return true;
    }
    return false;
}