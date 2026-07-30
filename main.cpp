#include "main.h"

int main() {
	
	Window window;

	std::unique_ptr<Aircraft> su27 = std::make_unique<Aircraft>(
		1.0f,
		0.0f, 1000.0f,         // 1, 2: direction_x, direction_y (Position in meters)
		400.0f,
		500.0f, 0.0f,          // 3, 4: speed_x, speed_y (Cruise velocity ~Mach 0.75 in m/s)
		1.3,                   // 5: drag_area (Equivalent zero-lift drag area in m²)
		62.0,                  // 6: lift_area (Wing reference area in m²)
		16500.0,               // 7: mass (Empty weight in kg)
		"Flanker-B",           // 8: nato_designation
		10.0,                  // 9: hardpoint_count (Su-27 has 10 weapon pylons)
		150440.0,              // 10: dry_engine_power (2x AL-31F engines dry thrust in N)
		245200.0,              // 11: wet_engine_power (2x AL-31F engines afterburner thrust in N)
		9400,                  // 12: max_fuel_load (Max internal fuel capacity in kg)
		9400,                  // 13: fuel_load (Current starting fuel in kg)
		2.85,                  // 14: dry_usage_sec (Fuel burn rate on dry power in kg/s)
		13.25                  // 15: wet_usage_sec (Fuel burn rate on afterburner in kg/s)
	);

	su27->toggle_aft();

	Shader shader_program("default.vert", "default.frag");
	glPointSize(50.0f);
	glLineWidth(3.0f); // call once in DrawAngleLine before glDrawArrays, or in init

	double last_frame = glfwGetTime();

	double sum = 0;

	while (!glfwWindowShouldClose(window.window)) {

		const double curr_frame = glfwGetTime();
		double delta_time = curr_frame - last_frame;
		last_frame = curr_frame;

		// Prevent physics explosions from lag spikes or window holds
		delta_time = std::min(delta_time, 0.05);

		sum += delta_time;

		su27->move(delta_time);

		window.Render(shader_program);

		window.DrawAngleLine(shader_program, su27->get_direction_x(), su27->get_direction_y(), su27->get_aoa(), 150.0f);
		
		glfwSwapBuffers(window.window);
		glfwPollEvents();
		
		std::cout << "\n" << sum << "seconds passed \n";

		if (sum >= 25) {
			break;
		}

	}
}
