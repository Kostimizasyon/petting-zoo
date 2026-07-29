#include "AerialObject.h"
#include "window_constants.h"
#include <iostream>

	AerialObject::AerialObject(GLfloat direction_x, GLfloat direction_y, GLfloat speed_x, GLfloat speed_y,
		double drag_area, double lift_area, double mass)
		: Renderable(direction_x, direction_y),
		direction_x(direction_x), direction_y(direction_y), speed_x(speed_x), speed_y(speed_y),
		drag_area(drag_area), lift_area(lift_area), mass(mass)
	{
		altitude = direction_y;
		temperature = calculate_temp(altitude);
		pressure = calculate_pressure(altitude);
		mach_number = speed_x / calculate_mach_speed(altitude);
	}

	void AerialObject::update_x_speed(const double drag_coefficient, const double dt) {
		const double air_density = calculate_air_density(altitude);
		const double drag = calculate_drag(drag_coefficient, air_density, speed_x, drag_area);

		const double push_force = get_push_force();

		// a = F / m
		const double acceleration_x = (push_force - drag) / mass;
		std::cout << drag << "\n";
		std::cout << acceleration_x * dt << "\n";
		speed_x += acceleration_x * dt;
	}

	void AerialObject::update_y_speed(const double lift_coefficient, const double dt) {

		const double air_density = calculate_air_density(altitude);
		const double lift = calculate_lift(lift_coefficient, air_density, speed_x, lift_area);
		const double gravity_force = calculate_gravity_force(mass);

		double acceleration_y = (lift - gravity_force) / mass;

		std::cout << "lift" << lift;

		speed_y += acceleration_y * dt;

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

		Update_Pos(direction_x, direction_y);
	}