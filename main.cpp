#include "main.h"

int main() {
	
	Window window;

	Aircraft su27(
		0.0f, 1000.0f,         // 1, 2: direction_x, direction_y (Position in meters)
		140.0f, 0.0f,          // 3, 4: speed_x, speed_y (Cruise velocity ~Mach 0.75 in m/s)
		1.3,                   // 5: drag_area (Equivalent zero-lift drag area in m²)
		62.0,                  // 6: lift_area (Wing reference area in m²)
		16500.0,               // 7: mass (Empty weight in kg)
		"Flanker-B",           // 8: nato_designation
		10.0,                  // 9: hardpoint_count (Su-27 has 10 weapon pylons)
		150440.0,              // 10: dry_engine_power (2x AL-31F engines dry thrust in N)
		245200.0,              // 11: wet_engine_power (2x AL-31F engines afterburner thrust in N)
		9400.0,                // 12: max_fuel_load (Max internal fuel capacity in kg)
		9400.0,                // 13: fuel_load (Current starting fuel in kg)
		2.85,                  // 14: dry_usage_sec (Fuel burn rate on dry power in kg/s)
		13.25                  // 15: wet_usage_sec (Fuel burn rate on afterburner in kg/s)
	);

	su27.toggle_aft();

	Shader shader_program("default.vert", "default.frag");
	glPointSize(15.0f);

	double lastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(window.window)) {

		const double currentFrame = glfwGetTime();
		double deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		su27.move(deltaTime);

		window.Render(shader_program);

		su27.Draw();
		
		glfwSwapBuffers(window.window);
		glfwPollEvents();
		
	}
}
