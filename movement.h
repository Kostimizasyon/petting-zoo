#pragma once
#include "AerialObject.h"

class MoveableAerialObject : public AerialObject {

	void update_x_speed(const double drag_coefficient, const double dt) {};

	void update_y_speed(const double lift_coefficient, const double dt);

	void update_parameters(float dt);

	void move(float dt);

	virtual void display_stats() = 0;

	virtual double get_push_force() = 0;

	virtual void decrement_dt() = 0;

};