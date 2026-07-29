#pragma once

#include <vector>
#include <iostream>
#include "AerialObject.h"
#include "Missile.h"

class Aircraft : public AerialObject {
protected:

    // Lodaout
    std::vector<Missile> loadout = {};

    std::string nato_designation = "";

    const double hardpoint_count = 0.0;

    // Engine Info
    const double dry_engine_power = 0.0;
    const double wet_engine_power = 0.0;
    bool is_aft = false;

    // Fuel Info & Burn Rates
    const double max_fuel_load = 0.0;
    double fuel_load = 0.0;
    const double dry_usage_sec = 0.0;
    const double wet_usage_sec = 0.0;

public:

    ~Aircraft() = default;

    Aircraft(const GLfloat direction_x,const GLfloat direction_y,const GLfloat speed_x,const GLfloat speed_y, double drag_area, double lift_area, double mass,
        const std::string& nato_designation, double hardpoint_count, double dry_engine_power, double wet_engine_power, double max_fuel_load,
        double fuel_load, double dry_usage_sec, double wet_usage_sec);

    // MY MAIN GOAL IS TO BLOW UP
    void explode();

    // Engine control
    void toggle_aft();

    // Missile Management
    void load_missile(const Missile& missile);

    void load_missiles(const std::vector<Missile>& missiles);

    // Missile Shoot
    void fire_missile(const float dt);
    void fire_missile_salvo(const short int amount, const float dt);

    // virtual implementation
    double get_push_force() override;

    void decrement_dt(const float dt) override;

    void display_stats() override;
};