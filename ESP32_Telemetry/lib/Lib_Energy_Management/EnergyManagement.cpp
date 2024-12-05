#include "EnergyManagement.hpp"

EnergyManagement energy_management_instance;

EnergyManagement::EnergyManagement(){
    memcpy(acceleration_point, accelerationPoints[acceleration_point_index], sizeof(accelerationPoints[acceleration_point_index]));
}


void EnergyManagement::loop(){
    updatePosition();
    // // check if caps are full, if yes, update pwm

    if(checkIfInAccelerationPoint()){
        // updatePwm(1);
        // requestAcceleration();
        
        measure_data.in_acceleration_point = 1;

        if(acceleration_point_index < ACCELERATION_POINTS_NUM){
            acceleration_point_index++;
        }else{
            acceleration_point_index = 0;
        }
        memcpy(acceleration_point, accelerationPoints[acceleration_point_index], sizeof(accelerationPoints[acceleration_point_index]));
        
        for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
            last_acceleration_distances[i] = MAX_DISTANCE;
        }

    }else if(checkIfInControlPoint()){
        updatePwm();
    }else{
        // measure_data.in_acceleration_point = 0;
    }
}


void EnergyManagement::updatePwm(uint16_t acceleration_point){
    if (acceleration_point){
        //TODO
    } else{
    uint16_t driving_time = drivingTimeToNextPoint();
    uint16_t required_energy = estimateOverallCapRequiredEnergy();

    uint16_t pwm_width = calculatePwmWidth(driving_time, required_energy);
    sendPwmWidth(pwm_width);
    }
}


uint16_t EnergyManagement::updatePosition(){
    current_position[LATITUDE_INDEX] = measure_data.latitude;
    current_position[LONGITUDE_INDEX] = measure_data.longitude;
    
    for(uint16_t i = 1; i < LAST_POINTS_NUM; ++i){
        last_acceleration_distances[i] = last_acceleration_distances[i-1];
    }

    last_acceleration_distances[0] = gps.distanceBetween(double(current_position[LATITUDE_INDEX]), double(current_position[LONGITUDE_INDEX]), double(acceleration_point[LATITUDE_INDEX]), double(acceleration_point[LONGITUDE_INDEX]));
    return 0;
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

    if(mean_distance < REQUIRED_DISTANCE){
        return true;
    }
    return false;
}