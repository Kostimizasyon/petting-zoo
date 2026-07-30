#include "AerialObject.h"
#include "window_constants.h"
#include <iostream>
#include <algorithm> // for std::clamp

AerialObject::AerialObject(GLfloat angle_of_attack, GLfloat direction_x, GLfloat direction_y, GLfloat max_speed, GLfloat speed_x, GLfloat speed_y,
	double drag_area, double lift_area, double mass)
	: Renderable(angle_of_attack, direction_x, direction_y), max_speed(max_speed),
	direction_x(direction_x), direction_y(direction_y), speed_x(speed_x), speed_y(speed_y),
	drag_area(drag_area), lift_area(lift_area), mass(mass)
{
	altitude = direction_y;
	temperature = calculate_temp(altitude);
	pressure = calculate_pressure(altitude);
	mach_number = speed_x / calculate_mach_speed(altitude);
}

// Adjusts angle_of_attack so lift stays roughly equal to weight (L ≈ W),
// mimicking a pilot/autopilot retrimming as speed changes, instead of
// letting lift blow up unbounded with v^2 at high speed.
void AerialObject::handle_trim() {
	if (trim_mode == NoTrim) return;

	const double air_density = calculate_air_density(altitude);
	const double total_air_speed = sqrt((speed_x * speed_x) + (speed_y * speed_y));
	const double gravity_force = calculate_gravity_force(mass);

	// mirrors the Cl selection rule used in move(); duplicated here since
	// handle_trim() runs before update_y_speed receives its own lift_coefficient
	const double lift_coefficient = mach_number >= 1 ? 0.2 : 0.15;

	if (total_air_speed < 1.0) return; // avoid divide-by-near-zero at rest/low speed

	// L = 0.5 * air_density * v^2 * (lift_area * aoa) * Cl  ==  gravity_force
	// solve for the aoa that makes that true
	const double required_aoa = gravity_force /
		(0.5 * air_density * total_air_speed * total_air_speed * lift_area * lift_coefficient);

	// crude stall clamp: real wings can't hold arbitrarily high aoa
	constexpr double MAX_AOA = 0.349; // ~20 degrees in radians
	angle_of_attack = static_cast<GLfloat>(std::clamp(required_aoa, -MAX_AOA, MAX_AOA));
}

void AerialObject::update_x_speed(const double drag_coefficient, const double dt) {

	const double air_density = calculate_air_density(altitude);

	const double total_air_speed = sqrt((speed_x * speed_x) + (speed_y * speed_y));

	const double effective_drag_area = (angle_of_attack == 0 ? 1 : angle_of_attack) * drag_area;

	const double drag = calculate_drag(drag_coefficient, air_density, total_air_speed, effective_drag_area);
	const double push_force = get_push_force();

	// a = F / m
	const double acceleration_x = (push_force - drag) / mass;

	speed_x += static_cast<GLfloat>(acceleration_x * dt);

	if (speed_x > max_speed) { speed_x = max_speed; return; }
	if (speed_x > window_const::LOGIC_SPEED_LIMIT_X) { speed_x = window_const::LOGIC_SPEED_LIMIT_X; return; }
}

void AerialObject::update_y_speed(const double lift_coefficient, const double dt) {

	const double air_density = calculate_air_density(altitude);

	const double total_air_speed = sqrt((speed_x * speed_x) + (speed_y * speed_y));

	handle_trim();

	const double effective_lift_area = (angle_of_attack == 0 ? 1 : angle_of_attack) * lift_area;

	const double lift = calculate_lift(lift_coefficient, air_density, total_air_speed, effective_lift_area);
	const double gravity_force = calculate_gravity_force(mass);

	double acceleration_y = (lift - gravity_force) / mass;

	speed_y += static_cast<GLfloat>(acceleration_y * dt);

	if (speed_y > window_const::LOGIC_SPEED_LIMIT_Y) speed_y = window_const::LOGIC_SPEED_LIMIT_Y;

}
void AerialObject::update_parameters(const float dt) {

	altitude += speed_y * dt;
	distance += speed_x * dt;

	mach_number = speed_x / calculate_mach_speed(altitude);

	temperature = calculate_temp(altitude);
	pressure = calculate_pressure(altitude);

}

void AerialObject::move(const float dt) {

	const double drag_coefficient = mach_number >= 1 ? 0.2 : 0.5;
	const double lift_coefficient = mach_number >= 1 ? 0.2 : 0.15;

	update_x_speed(drag_coefficient, dt);
	update_y_speed(lift_coefficient, dt);
	update_parameters(dt);

	direction_x += speed_x * dt;
	direction_y += speed_y * dt;

	direction_x = direction_x > window_const::WORLD_WIDTH ? direction_x - window_const::WORLD_WIDTH : direction_x;
	direction_y = direction_y > window_const::WORLD_HEIGHT ? direction_y - window_const::WORLD_HEIGHT : direction_y;

	decrement_dt(dt);

	display_stats();

	Update_Pos(angle_of_attack, direction_x, direction_y);
}