#ifndef ENERGY_MANAGEMENT_HPP
#define ENERGY_MANAGEMENT_HPP

#include "DataManagement.hpp"
#include "GNSS.hpp"

#define LATITUDE_INDEX 0
#define LONGITUDE_INDEX 1
#define ENERGY_INDEX 2

#define ACCELERATION_POINTS_NUM 1
#define LIST_SIZE 3

#define MAX_DISTANCE 999
#define LAST_POINTS_NUM 3
#define REQUIRED_DISTANCE 15.0 //m

static uint16_t acceleration_point_index = 0;
constexpr float accelerationPoints[ACCELERATION_POINTS_NUM][LIST_SIZE] = {
    {50.043930, 19.95648, 234.0} //latitude, longitude, energy
};

#define CONTROL_POINTS_NUM 6
constexpr float controlPoints[CONTROL_POINTS_NUM][LIST_SIZE] = {
    {50.0591731,  19.9218765, 234.0}, //szerokość(latitude), wysokość(longitude), time to the next point
    {50.0591839,19.9215888},
    {50.059186, 19.92134},
    {50.0591899, 19.9210128},
    {50.0591936, 19.9207262},
    {50.0592098, 19.9203275}
};

struct{
    float min_cap_constraint;
    float max_cap_constraint;
    
    uint16_t fuel_cell_min_power;
    uint16_t fuel_cell_max_power;

    uint16_t converter_min_power;
    uint16_t converter_max_power;

} EnergyManagementConstraints;

class EnergyManagement{

public:
    EnergyManagement();
    void loop();
    uint16_t updatePosition();
    void updatePwm(uint16_t acceleration_point = 0);

    uint16_t drivingTimeToNextPoint();
    uint16_t estimateOverallCapRequiredEnergy();
    uint16_t calculatePwmWidth(uint16_t driving_time, uint16_t required_energy);
    void sendPwmWidth(uint16_t pwm_width );

    void hustonWeGotProblem(); //called if point was missed

    bool checkIfInAccelerationPoint();
    bool checkIfInControlPoint();
    bool requestAcceleration();
    void sendAccelerationCommand();


private:
    float acceleration_point[LIST_SIZE];
    float control_point[LIST_SIZE];
    float current_position[2];
    uint16_t last_acceleration_distances[LAST_POINTS_NUM] = {MAX_DISTANCE};  //keeps last 3 meassured distances from current acceleration point

    float current_speed;
    float cap_desired_energy;
    float cap_voltage_measurement;

    uint16_t control_point_index;
    
};

extern EnergyManagement energy_management_instance;
#endif //ENERGY_MANAGEMENT_HPP