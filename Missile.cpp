#include "Missile.h"
#include "Aircraft.h"
#include <algorithm>

Missile::Missile(const GLfloat angle_of_attack, const GLfloat direction_x, const GLfloat direction_y,
    const GLfloat max_speed, const GLfloat speed_x, const GLfloat speed_y,
    double drag_area, double lift_area, double dry_mass,
    const std::string& nato_designation,
    double engine_power, double engine_time, double engine_fuel,
    double sustainer_power, double sustainer_time, double sustainer_fuel)
    : AerialObject(angle_of_attack, direction_x, direction_y, max_speed, speed_x, speed_y, drag_area, lift_area, dry_mass + engine_fuel + sustainer_fuel),
    dry_mass(dry_mass),
    nato_designation(nato_designation),
    engine_power(engine_power),
    engine_time(engine_time),
    initial_engine_time(engine_time),
    engine_fuel(engine_fuel),
    initial_engine_fuel(engine_fuel),
    sustainer_power(sustainer_power),
    sustainer_time(sustainer_time),
    initial_sustainer_time(sustainer_time),
    sustainer_fuel(sustainer_fuel),
    initial_sustainer_fuel(sustainer_fuel)
{
    set_mass(dry_mass + engine_fuel + sustainer_fuel);
}

//MY MAIN GOAL IS TO BLOW UP
void Missile::explode() {
    this->~Missile();
}

// Get shot
void Missile::launch(const Aircraft& launch_aircraft, const double dt) {
    // 1. Inherit position (X & Y / Altitude) from the aircraft
    set_direction_x(launch_aircraft.get_direction_x());
    set_direction_y(launch_aircraft.get_direction_y() - 5.0f, false);

    // 2. Inherit initial velocity vector (Speed X & Y) from the aircraft
    set_speed_x(launch_aircraft.get_speed_x(), false);
    set_speed_y(launch_aircraft.get_speed_y(), false);

    recalculate_environment();

    move(dt);
}

// Implementing virtual
double Missile::get_push_force() {
    if (engine_time > 0)             return engine_power;
    else if (sustainer_time > 0)     return sustainer_power;
    else                             return 0;
}

void Missile::decrement_dt(const double dt) {
    double time_remaining = dt;

    if (engine_time > 0 && time_remaining > 0) {
        double burn_duration = std::min(static_cast<double>(time_remaining), engine_time);

        double burn_rate = (initial_engine_time > 0) ? (initial_engine_fuel / initial_engine_time) : 0.0;
        double fuel_burned = burn_rate * burn_duration;

        engine_fuel = std::max(0.0, engine_fuel - fuel_burned);
        engine_time -= burn_duration;
        time_remaining -= burn_duration;
    }

    if (engine_time <= 0 && sustainer_time > 0 && time_remaining > 0) {
        double burn_duration = std::min(static_cast<double>(time_remaining), sustainer_time);

        double burn_rate = (initial_sustainer_time > 0) ? (initial_sustainer_fuel / initial_sustainer_time) : 0.0;
        double fuel_burned = burn_rate * burn_duration;

        sustainer_fuel = std::max(0.0, sustainer_fuel - fuel_burned);
        sustainer_time -= burn_duration;
        time_remaining -= burn_duration;
    }

    set_mass(dry_mass + engine_fuel + sustainer_fuel);
}

void Missile::display_stats() {
    std::cout << "--- " << nato_designation << " TELEMETRY---\n"
        << "Phase:         " << (engine_time > 0 ? "BOOST" : sustainer_time > 0 ? "SUSTAIN" : "GLIDE") << "\n"
        << "Speed X:       " << std::fixed << std::setprecision(2) << speed_x << " m/s (" << (speed_x * 3.6) << " km/h)\n"
        << "Speed Y:       " << speed_y << " m/s\n"
        << "Mach:          " << std::setprecision(3) << mach_number << "\n"
        << "Altitude:      " << (int)altitude << " m\n"
        << "Total Mass:    " << std::setprecision(2) << mass << " kg\n"
        << "Engine Fuel:   " << engine_fuel << " kg\n"
        << "Sustainer Fuel:" << sustainer_fuel << " kg\n"
        << "-------------------------\n";
}