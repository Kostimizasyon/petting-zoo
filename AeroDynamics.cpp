#include "AeroDynamics.h"

//In aerodynamics we have 5 main forces affecting our object, lift vs gravity and drag vs engine power and air friction on top of all that, to make the object
//move in air we need to make sure that lift and engine power are winning out.

/*
		LIFT
		||
	THRUST<= OBJECT => DRAG FORCE
		||
		GRAVITY

*/

//Air density isnt something exactly that we calculate, we moreso look at it as something that affects our lift and drag 1:1 almost, if it halves the lift and drag halves, so what exactly does it depend on?

//PV=NRT => n / V = density so ::: density = P / RT :::: and since R is a constant we dotn care about it => p = P / T ::::

//So the main aspects we need to look at are heat and pressure::::


//Both lift and drag are a mechanical force, therefore they need physical contact to affect objects => NO FLUID NO FORCE, they soley rely on fluids to generate force, be it liquid or 
//gas. In addition to the needs of liquid, both these forces also rely on speed, so NO MOTION NO FORCE


//Drag formula = Dragcoefficient * density of air * Speed**2 / 2 * reference area
double AeroDynamics::calculate_drag(const double drag_coefficient, const double air_density, const double speed, const double reference_area) const {

	return (drag_coefficient * air_density * (speed * speed * 0.5) * reference_area);

}

//Lift Formula = Liftcoefficient * density of air * Speed**2 / 2 * wing area
double AeroDynamics::calculate_lift(const double lift_coefficient, const double air_density, const double speed, const double reference_area) const {

	return (lift_coefficient * air_density * (speed * speed * 0.5) * reference_area);

}

double AeroDynamics::calculate_gravity_force(const double mass) const {

	return (mass * GRAVITY_CONSTANT);

}

double AeroDynamics::calculate_mach_speed(const double altitude) const {

	return sqrt((GAMMA_CONSTANT * STAR_GAS_CONSTANT * calculate_temp(altitude)));

}

double AeroDynamics::calculate_temp(const double altitude) const {
	return (TEMPERATURE - (LAPSE_RATE * altitude));

}

//Atmospheric pressure = P0(sea level pressure) * ( 1 - (L (temperature lapse per m) * h / T0) ** - g * M (molar mass of dry air) / gas constant * L
double AeroDynamics::calculate_pressure(const double altitude) const {

	double t_val = calculate_temp(altitude);
	// Exponent calculation: g * M / (R * L)
	double exponent = (GRAVITY_CONSTANT * MOLAR_MASS_AIR) / (GAS_CONSTANT * LAPSE_RATE);
	return SEA_LEVEL_PRESSURE * std::pow((t_val / SEA_LEVEL_TEMP), exponent);

}

//Air density : pM / RT => atmosphericPressure * molar mass of gas / Gas Constant * Temperature
double AeroDynamics::calculate_air_density(const double altitude) const {

	double p_val = calculate_pressure(altitude);
	double t_val = calculate_temp(altitude);

	return (p_val * 0.02896968) / (GAS_CONSTANT * t_val);

}

double AeroDynamics::to_knt(const double speed) const {
	return 1.94384 * speed;
}