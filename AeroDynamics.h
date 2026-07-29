#pragma once
#include <cmath>

constexpr double GAMMA_CONSTANT = 1.4;
constexpr double GAS_CONSTANT = 8.31446;
constexpr double STAR_GAS_CONSTANT = 287.055023;
constexpr float GRAVITY_CONSTANT = 9.81f;
constexpr double TEMPERATURE = 293.0;
constexpr double SEA_LEVEL_TEMP = 288.15;     // Standard is 15°C / 288.15K
constexpr double SEA_LEVEL_PRESSURE = 101325.0; // Pascals
constexpr double LAPSE_RATE = 0.0065;
constexpr double MOLAR_MASS_AIR = 0.02896968;

class AeroDynamics {
public:
    virtual ~AeroDynamics() = default;

    // Aerodynamic Force Calculations
    double calculate_drag(const double drag_coefficient, const double air_density, const double speed, const double reference_area) const;
    double calculate_lift(const double lift_coefficient, const double air_density, const double speed, const double reference_area) const;
    double calculate_gravity_force(const double mass) const;

    // Environmental / Atmospheric Calculations
    double calculate_mach_speed(const double altitude) const;
    double calculate_temp(const double altitude) const;
    double calculate_pressure(const double altitude) const;
    double calculate_air_density(const double altitude) const;

    // Unit Conversions
    double to_knt(const double speed) const;
};