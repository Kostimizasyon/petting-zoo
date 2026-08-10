#pragma once

#include <glad/glad.h>
#include "AeroDynamics.h"
#include "Renderable.h"

enum Trim {
    LevelFlight,
    NoTrim
};

class AerialObject : public AeroDynamics, public Renderable {
protected:
    
    //Position
    GLfloat angle_of_attack = 0.0f;
    
    GLfloat direction_x = 0.0f;
    GLfloat direction_y = 0.0f;

    //Info
    GLfloat max_speed = 0.0f;
    GLfloat speed_x = 0.0f;
    GLfloat speed_y = 0.0f;
    double altitude = 0;
    double distance = 0;
    double mach_number = 0;

    //Misc
    const double drag_area = 0;
    const double lift_area = 0;
    double mass = 0;
    double temperature = 0;
    double pressure = 0;

    Trim trim_mode = Trim::LevelFlight;

    // Helper to keep atmospheric and speed metrics in sync
    void recalculate_environment() {
        temperature = calculate_temp(altitude);
        pressure = calculate_pressure(altitude);

        double mach_spd = calculate_mach_speed(altitude);
        // Uses total speed magnitude sqrt(speed_x^2 + speed_y^2)
        double total_speed = std::hypot(speed_x, speed_y);

        mach_number = (mach_spd > 0.0) ? (total_speed / mach_spd) : 0.0;
    }

public:
    AerialObject(GLfloat angle_of_attack, GLfloat direction_x, GLfloat direction_y,GLfloat max_speed, GLfloat speed_x, GLfloat speed_y,
        double drag_area, double lift_area, double mass);

    virtual ~AerialObject() = default;

    void update_x_speed(const double drag_coefficient, const double dt);

    void update_y_speed(const double lift_coefficient, const double dt);

    void update_parameters(double dt);

    void handle_trim();

    void move(double dt);

    void pull_aoa(const float aoa);

    virtual void display_stats() = 0;

    virtual double get_push_force() = 0;

    virtual void decrement_dt(const float dt) = 0;

    // Getters for position and basic state
    GLfloat get_aoa() const { return angle_of_attack;  }
    GLfloat get_direction_x() const { return direction_x; }
    GLfloat get_direction_y() const { return direction_y; }
    GLfloat get_speed_x() const { return speed_x; }
    GLfloat get_speed_y() const { return speed_y; }
    Trim get_trim() const { return trim_mode; }
    double get_mass() const { return mass; }
    double get_altitude() const { return altitude; }

    // Setters (with optional bool update_env = true)
    void set_aoa(GLfloat aoa) { angle_of_attack = aoa; }

    void set_trim(Trim mode) { trim_mode = mode; }

    void set_direction_x(GLfloat x) { direction_x = x; }

    void set_direction_y(GLfloat y, bool auto_update = true) {
        direction_y = y;
        altitude = static_cast<double>(y);
        if (auto_update) recalculate_environment();
    }

    void set_altitude(double a, bool auto_update = true) {
        altitude = a;
        direction_y = static_cast<GLfloat>(a);
        if (auto_update) recalculate_environment();
    }

    void set_speed_x(GLfloat x, bool auto_update = true) {
        speed_x = x;
        if (auto_update) recalculate_environment();
    }

    void set_speed_y(GLfloat y, bool auto_update = true) {
        speed_y = y;
        if (auto_update) recalculate_environment();
    }

    void set_mass(double m) { mass = m; }
};