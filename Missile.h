#pragma once

#include <iostream>
#include <iomanip>
#include "AerialObject.h"

class Aircraft;

class Missile : public AerialObject {
protected:

    const std::string nato_designation = "";

    // Fuel Info
    const double max_fuel_load = 0.0;
    double fuel_load = 0.0;

    // Engine Stages
    const double engine_power = 0.0;
    double engine_time = 0.0;
    const double sustainer_power = 0.0;
    double sustainer_time = 0.0;

public:

    ~Missile() = default;

    Missile(const GLfloat direction_x, const GLfloat direction_y, const GLfloat speed_x, const GLfloat speed_y, double drag_area, double lift_area, double mass,
        const std::string& nato_designation,
        double engine_power, double engine_time, double sustainer_power, double sustainer_time);

    void explode();

    // Launch
    void launch(const Aircraft& launch_aircraft, const float dt);

    // Virtual
    double get_push_force() override;

    void decrement_dt(const float dt) override;

    void display_stats() override;
};