#ifndef ENERGY_MANAGEMENT_HPP
#define ENERGY_MANAGEMENT_HPP

#include "DataManagement.hpp"
#include "GNSS.hpp"

#define CAPACITOR_MOSFET_PWM 13       // Pin, na którym podłączono diodę LED
const int mosfet_pwm_channel = 0;  // Kanał PWM (0-15)
const int pwmFreq = 2000;  // Częstotliwość PWM w Hz
const int pwmResolution = 8; // Rozdzielczość PWM (8-bitowa)


#define LATITUDE_INDEX 0
#define LONGITUDE_INDEX 1
#define VOLTAGE_INDEX 2

#define ACCELERATION_POINT 1
#define CAPS_FULL_POINT 2


#define LIST_SIZE 3

#define MAX_DISTANCE 999
#define LAST_POINTS_NUM 3
#define REQUIRED_DISTANCE 15.0 //m


constexpr float accelerationPoints[][LIST_SIZE] = {
    {50.065146,	    19.9161853, 44},  //latitude, longitude, energy [V]
    {50.0646205,	19.9183928, 44}, 
    {50.0644019,	19.9196652, 44}, 

};
constexpr uint16_t acceleration_points_num = sizeof(accelerationPoints)/sizeof(accelerationPoints[0]);
static uint16_t acceleration_point_index = 0;


constexpr float controlPoints[][LIST_SIZE] = {
    {50.0655481,	19.9145114, 30}, //szerokość(latitude), wysokość(longitude), time to the next point
    {50.0654103,	19.9150639, 20},
    {50.0652915,	19.9155762, 10},
    {50.0650045,	19.9167623, 30},
    {50.0648874,	19.9172612, 20},
    {50.0647532,	19.9178424, 10},
    {50.0644964,	19.9190058, 10}
};
static const uint16_t time_position = 2;
constexpr uint16_t control_points_num = sizeof(controlPoints)/sizeof(controlPoints[0]);
static uint16_t control_point_index = 0;

// simulation charger
constexpr float simulation_charge_state[] = {40.0,
                                            41.5,
                                            43.0,
                                            42.0,
                                            42.9,
                                            44.0,   
                                            41.0,
};


struct{
    float min_cap_constraint;
    float max_cap_constraint = 44.0;
    
    uint16_t fuel_cell_min_power = 180;
    uint16_t fuel_cell_max_power = 269;
    uint16_t optimal_accelerate_power = 250;

    uint16_t converter_min_power;
    uint16_t converter_max_power;

} EnergyManagementConstraints;

class EnergyManagement{

public:
    EnergyManagement();
    void loop();
    void updatePosition();
    void updatePwm(uint16_t acceleration_point = 0);

    uint16_t drivingTimeToNextPoint();
    uint16_t estimateChargePower();
    uint16_t choosePower(float charge_rata);
    uint16_t calculatePwmWidth(uint16_t chargePower);
    void sendPwmWidth(uint16_t pwm_width );

    void hustonWeGotProblem(); //called if point was missed

    bool checkIfInAccelerationPoint();
    bool checkIfInControlPoint();

    void requestAcceleration();
    void accelerate();


private:
    float acceleration_point[LIST_SIZE];
    float control_point[LIST_SIZE];
    float current_position[2];
    uint16_t last_control_distances[LAST_POINTS_NUM];
    uint16_t last_acceleration_distances[LAST_POINTS_NUM];  //keeps last 3 meassured distances from current acceleration point

    float current_speed;
    float cap_desired_energy;
    float cap_voltage_measurement;
    
};

extern EnergyManagement energy_management_instance;
#endif //ENERGY_MANAGEMENT_HPP