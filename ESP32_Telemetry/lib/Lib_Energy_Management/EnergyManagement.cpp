#include "EnergyManagement.hpp"

EnergyManagement energy_management_instance;

EnergyManagement::EnergyManagement(){
    memcpy(acceleration_point, accelerationPoints[acceleration_point_index], sizeof(accelerationPoints[acceleration_point_index]));
    memcpy(control_point, controlPoints[control_point_index], sizeof(controlPoints[control_point_index]));

    for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
        last_acceleration_distances[i] = MAX_DISTANCE;
        last_control_distances[i] = MAX_DISTANCE;
    }

    ledcSetup(mosfet_pwm_channel, pwmFreq, pwmResolution);
    ledcAttachPin(CAPACITOR_MOSFET_PWM, mosfet_pwm_channel);
}


void EnergyManagement::loop(){
    updatePosition();
    // // check if caps are full, if yes, update pwm
    if(measure_data.voltage_measurement[ADS_ADRESS_BEHIND_CELL] >= EnergyManagementConstraints.max_cap_constraint){
        //TODO
    }

    if(checkIfInAccelerationPoint()){
        requestAcceleration();
        
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
    Serial.println();
    Serial.println("acceleration_point_index");
    Serial.println(acceleration_point_index);
    Serial.println();
}


void EnergyManagement::requestAcceleration(){
    // alert driver
}


void EnergyManagement::accelerate(){
    updatePwm(1);
    // accelerate
}


void EnergyManagement::updatePosition(){
    current_position[LATITUDE_INDEX] = measure_data.latitude;
    current_position[LONGITUDE_INDEX] = measure_data.longitude;
    
    if(current_position[LATITUDE_INDEX] == 0.0 || current_position[LONGITUDE_INDEX] == 0.0){
        return;
    }

    for(uint16_t i = 0; i < LAST_POINTS_NUM - 1; ++i){
        last_acceleration_distances[i] = last_acceleration_distances[i+1];
        last_control_distances[i] = last_control_distances[i+1];
    }

    last_acceleration_distances[LAST_POINTS_NUM-1] = gps.distanceBetween(double(current_position[LATITUDE_INDEX]), double(current_position[LONGITUDE_INDEX]), double(acceleration_point[LATITUDE_INDEX]), double(acceleration_point[LONGITUDE_INDEX]));
    last_control_distances[LAST_POINTS_NUM-1] = gps.distanceBetween(double(current_position[LATITUDE_INDEX]), double(current_position[LONGITUDE_INDEX]), double(control_point[LATITUDE_INDEX]), double(control_point[LONGITUDE_INDEX]));
}


bool EnergyManagement::checkIfInAccelerationPoint(){
    float mean_distance = 0.0;
    for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
        mean_distance += last_acceleration_distances[i];
    }
    mean_distance /= float(LAST_POINTS_NUM);
    
    Serial.println();
    Serial.println("acceleration point distance");
    Serial.println(mean_distance);
    Serial.println();

    if(mean_distance < REQUIRED_DISTANCE){
        return true;
    }
    return false;
}


bool EnergyManagement::checkIfInControlPoint(){
    float mean_distance = 0.0;
    for(uint16_t i = 0; i < LAST_POINTS_NUM; ++i){
        mean_distance += float(last_control_distances[i]);
    }
    mean_distance /= float(LAST_POINTS_NUM);
    
    Serial.println();
    Serial.println("control point distance");
    Serial.println(mean_distance);
    Serial.println();

    if(mean_distance < REQUIRED_DISTANCE){
        return true;
    }
    return false;
}


uint16_t EnergyManagement::drivingTimeToNextPoint(){
    return control_point[time_position];
}


void EnergyManagement::updatePwm(uint16_t acceleration_point){
    uint16_t pwm_width;
    if (acceleration_point){
        pwm_width = calculatePwmWidth(EnergyManagementConstraints.optimal_accelerate_power);
    } else{
        uint16_t chargePower = estimateChargePower();
        pwm_width = calculatePwmWidth(chargePower);
    }
    sendPwmWidth(pwm_width);
}


uint16_t EnergyManagement::estimateChargePower(){
    uint16_t driving_time = drivingTimeToNextPoint();
    // float cap_voltage = measure_data.voltage_measurement[ADS_ADRESS_BEHIND_CELL];
    float cap_voltage = 40;

    float desired_voltage = acceleration_point[VOLTAGE_INDEX];
    float differencial = desired_voltage - cap_voltage;

    //TODO: what if caps will discharge (doesnt make sens to charge 1V)
    if(cap_voltage == EnergyManagementConstraints.max_cap_constraint){
        return 0;
    }else{
        float charge_rate = differencial/driving_time;
        return choosePower(charge_rate);
    }
}


//TODO: set proper values
uint16_t EnergyManagement::choosePower(float charge_rate){
    if(charge_rate > 0.3){
        return EnergyManagementConstraints.fuel_cell_max_power;
    }else if (charge_rate > 0.2){
        return EnergyManagementConstraints.fuel_cell_max_power - 20;
    }else if (charge_rate > 0.1){
        return EnergyManagementConstraints.fuel_cell_min_power + 20;
    }else if (charge_rate > 0){
        return EnergyManagementConstraints.fuel_cell_min_power + 10;
    }else{
        return EnergyManagementConstraints.fuel_cell_min_power;
    }

    return 0;
}





uint16_t EnergyManagement::calculatePwmWidth(uint16_t chargePower ){
// if caps are full or near full, set to 0
    return chargePower;
}


void EnergyManagement::sendPwmWidth(uint16_t pwm_width ){
    measure_data.pwm_width = pwm_width;
    // ledcWrite(mosfet_pwm_channel, pwm_width);

    Serial.println();
    Serial.println("fuel cell power");
    Serial.println(pwm_width);
    Serial.println();
}


