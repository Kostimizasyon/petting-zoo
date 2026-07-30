#pragma once

#include <iostream>
#include <iomanip>
#include "AerialObject.h"

class Aircraft;

class Missile : public AerialObject {
protected:
    std::string nato_designation = "";

    double dry_mass = 0.0;

    // Engine Stages & Fuel
    double engine_power = 0.0;
    double engine_time = 0.0;
    double initial_engine_time = 0.0;
    double engine_fuel = 0.0;
    double initial_engine_fuel = 0.0;

    double sustainer_power = 0.0;
    double sustainer_time = 0.0;
    double initial_sustainer_time = 0.0;
    double sustainer_fuel = 0.0;
    double initial_sustainer_fuel = 0.0;

public:
    ~Missile() = default;

    Missile(const GLfloat angle_of_attack, const GLfloat direction_x, const GLfloat direction_y, const GLfloat max_speed, const GLfloat speed_x, const GLfloat speed_y, double drag_area, double lift_area, double dry_mass,
        const std::string& nato_designation,
        double engine_power, double engine_time, double engine_fuel,
        double sustainer_power, double sustainer_time, double sustainer_fuel);

    void explode();

    // Launch
    void launch(const Aircraft& launch_aircraft, const float dt);

    // Virtual
    double get_push_force() override;

    void decrement_dt(const float dt) override;

    void display_stats() override;
};