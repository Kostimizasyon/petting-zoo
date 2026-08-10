#include "Aircraft.h"

	Aircraft::Aircraft(const GLfloat angle_of_attack, const GLfloat direction_x, const GLfloat direction_y, const GLfloat max_speed, const GLfloat speed_x, const GLfloat speed_y, double drag_area, double lift_area, double mass,
		const std::string& nato_designation, double hardpoint_count, double dry_engine_power, double wet_engine_power, double max_fuel_load,
		double fuel_load, double dry_usage_sec, double wet_usage_sec) :
		AerialObject(angle_of_attack, direction_x, direction_y, max_speed, speed_x, speed_y, drag_area, lift_area, mass),
		nato_designation(nato_designation),hardpoint_count(hardpoint_count),dry_engine_power(dry_engine_power), wet_engine_power(wet_engine_power),
		max_fuel_load(max_fuel_load),fuel_load(fuel_load), dry_usage_sec(dry_usage_sec), wet_usage_sec(wet_usage_sec)
	{
		set_mass((mass += fuel_load));
	}

	// MY MAIN GOIL IS TO BLOW UP
	void Aircraft::explode() {
		this->~Aircraft();
	}

	// Engine Control
	void Aircraft::toggle_aft() {
		is_aft = !is_aft;
	}

	// Missile Management 

	void Aircraft::load_missile(const Missile& missile) {
		if (this->loadout.size() < this->hardpoint_count - 2) {
			// need symteric loads
			this->loadout.emplace_back(missile);
			this->loadout.emplace_back(missile);
			this->mass += (2 * missile.get_mass());
		}
		else {
			std::cout << "Cant load any more missiles.";
		}
	}

	void Aircraft::load_missiles(const std::vector<Missile>& missles) {
		for (const Missile& missile : missles) {
			load_missile(missile);
		}
	}

	void Aircraft::fire_missile(const double dt) {

		if (loadout.empty()) {
			std::cout << "No missiles to fire!";
			return;
		}

		else {

			Missile to_fire = loadout.back();
			loadout.pop_back();
			to_fire.launch(*this, dt);

			set_mass(mass -= to_fire.get_mass());
		}
	}

	void Aircraft::fire_missile_salvo(const short int amount, const double dt) {

		for (int i = 0; i < amount; i++) {
			fire_missile(dt);
		}

	}

	// virtual implementation

	double Aircraft::get_push_force() {
		return wet_engine_power;
		if (is_aft && fuel_load > 0) {
			return wet_engine_power;
		}
		else if (!is_aft && fuel_load > 0) {
			return dry_engine_power;
		}
		else {
			return 0;
		}
	}

	void Aircraft::decrement_dt(const dobule dt) {
		if (is_aft && fuel_load > 0) {
			fuel_load -= wet_usage_sec * dt;
			set_mass(mass -= wet_usage_sec * dt);
		}
		else if (!is_aft && fuel_load > 0) {
			fuel_load -= dry_usage_sec * dt;
			set_mass(mass -= dry_usage_sec * dt);
		}
	}

	void Aircraft::display_stats() {
	
		std::cout << "--- "
			<< nato_designation
			<< " TELEMETRY---" << "\n"
			<< "Burner:    " << (is_aft ? "AFT" : "DRY") << "\n"
			<< "Speed X:  " << std::fixed << std::setprecision(2) << speed_x << " m/s (" << (speed_x * 3.6) << " km/h)\n"
			<< "Speed Y:  " << speed_y << " m/s\n"
	        << "Mach:     " << std::setprecision(3) << mach_number << "\n"
			<< "Altitude: " << (int)altitude << " m"
			<< "-------------------------" << "\n";

	}