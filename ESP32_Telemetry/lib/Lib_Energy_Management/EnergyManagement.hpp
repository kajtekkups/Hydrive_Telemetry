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


#define LIST_SIZE 3

#define MAX_DISTANCE 999
#define LAST_POINTS_NUM 3
#define REQUIRED_DISTANCE 15.0 //m


constexpr float accelerationPoints[][LIST_SIZE] = {
    {51.043930, 19.95648, 48.0}, //latitude, longitude, energy [V]
    {50.043930, 19.95648, 45.0}, //latitude, longitude, energy [V]
    {50.043930, 19.95648, 44.0}, //latitude, longitude, energy [V]
    {50.043930, 19.95648, 48.0}, //latitude, longitude, energy [V]
    {50.043930, 19.95648, 40.0}, //latitude, longitude, energy [V]
};
constexpr uint16_t acceleration_points_num = sizeof(accelerationPoints)/sizeof(accelerationPoints[0]);
static uint16_t acceleration_point_index = 0;


constexpr float controlPoints[][LIST_SIZE] = {
    {50.043930, 19.95648, 15.0}, //szerokość(latitude), wysokość(longitude), time to the next point
    {50.043930, 19.95648, 40.0}, //latitude, longitude
    {50.043930, 19.95648, 10.0}, //latitude, longitude 
    {50.043930, 19.95648, 100.0}, //latitude, longitude
    {50.043930, 19.95648, 25.0}, //latitude, longitude
    {50.043930, 19.95648, 15.0}, //latitude, longitude
};
static const uint16_t time_position = 2;
constexpr uint16_t control_points_num = sizeof(controlPoints)/sizeof(controlPoints[0]);
static uint16_t control_point_index = 0;

struct{
    float min_cap_constraint;
    float max_cap_constraint;
    
    uint16_t fuel_cell_min_power = 180;
    uint16_t fuel_cell_max_power = 269;

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
    bool requestAcceleration();
    void sendAccelerationCommand();


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