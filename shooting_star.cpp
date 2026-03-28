#include "main.h"
#define GAMMA_CONSTANT 1.4
#define GAS_CONSTANT 8.31446
#define STAR_GAS_CONSTANT 287.055023
#define GRAVITY_CONSTANT 9.81f
#define TEMPERATURE 293
#define SEA_LEVEL_TEMP 288.15  // Standard is 15°C / 288.15K
#define SEA_LEVEL_PRESSURE 101325.0 // Pascals
#define LAPSE_RATE 0.0065
#define MOLAR_MASS_AIR 0.02896968

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080	

constexpr float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
constexpr float WORLD_HEIGHT = 10000;
constexpr float WORLD_WIDTH = WORLD_HEIGHT * aspectRatio;

struct Window {
	GLFWwindow* window;
	int WIDTH = WINDOW_WIDTH;
	int HEIGHT = WINDOW_HEIGHT;
	
	Window() {
			
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Shootin Missile", NULL, NULL);

		if (window == NULL) {
			std::cout << "Window failed to create \n";
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);

		gladLoadGL();

		glViewport(0, 0, WIDTH, HEIGHT);

		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int width, int height) {
			glViewport(0, 0, width, height);
		});

	}
};

class AeroDynamics {
public:
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
	double calculateDrag(double dragCoefficient, double airDensity, double speed, double referenceArea) {

		return (dragCoefficient * airDensity * (pow(speed, 2) / 2) * referenceArea);

	}

	//Lift Formula = Liftcoefficient * density of air * Speed**2 / 2 * wing area
	double calculateLift(double liftCoeeficient, double airDensity, double speed, double referenceArea) {

		return (liftCoeeficient * airDensity * (pow(speed, 2) / 2) * referenceArea);

	}

	double calculateGravityForce(double mass) {

		return (mass * GRAVITY_CONSTANT);

	}


	double calculateMachSpeed(double altitude) {

		return sqrt((GAMMA_CONSTANT * STAR_GAS_CONSTANT * calculateTemp(altitude)));

	}

	double calculateTemp(double altitude) {
		return (TEMPERATURE - (LAPSE_RATE * altitude));

	}

	//Atmospheric pressure = P0(sea level pressure) * ( 1 - (L (temperature lapse per m) * h / T0) ** - g * M (molar mass of dry air) / gas constant * L

	double calculatePressure(double altitude) {

		double T = calculateTemp(altitude);
		// Exponent calculation: g * M / (R * L)
		double exponent = (GRAVITY_CONSTANT * MOLAR_MASS_AIR) / (GAS_CONSTANT * LAPSE_RATE);
		return SEA_LEVEL_PRESSURE * std::pow((T / SEA_LEVEL_TEMP), exponent);

	}

	//Air density : pM / RT => atmosphericPressure * molar mass of gas / Gas Constant * Temperature
	double calculateAirDensity(double altitude) {

		double P = calculatePressure(altitude);
		double T = calculateTemp(altitude);

		return (P * 0.02896968) / (GAS_CONSTANT * T);

	}

	double toKNT(double speed) {
		return 1.94384 * speed;
	}

};

class Missile :AeroDynamics {
public:
	//Position
	GLfloat directionX = 0.0f;
	GLfloat directionY = 0.0f;

	//Info
	GLfloat speedX = 0.0f;
	GLfloat speedY = 0.0f;
	double altitude = 0;
	double distance = 0;
	double machNumber = 0;

	//Engine info
	double enginePower = 0;
	double engineTime = 0;

	//Sustainer info
	double sustainerPower = 0;
	double sustainerTime = 0;

	//Misc
	double dragArea = 0;
	double liftArea = 0;
	double mass = 0;
	double temperature = 0;
	double pressure = 0;

	Missile(GLfloat directionX, GLfloat directionY, GLfloat speedX, GLfloat speedY,
		double enginePower, double engineTime, double sustainerPower, double sustainerTime,
		double dragArea, double liftArea, double mass)
		: directionX(directionX), directionY(directionY), speedX(speedX), speedY(speedY),
		enginePower(enginePower), engineTime(engineTime),
		sustainerPower(sustainerPower), sustainerTime(sustainerTime),
		dragArea(dragArea), liftArea(liftArea), mass(mass)
	{
		altitude = directionY;

		temperature = calculateTemp(altitude);
		pressure = calculatePressure(altitude);
		machNumber = speedX / calculateMachSpeed(altitude);
	}

	void updateXSpeed(double dragCoefficient, double dt) {
		double airDensity = calculateAirDensity(altitude);
		double drag = calculateDrag(dragCoefficient, airDensity, speedX, dragArea);

		// Determine which stage of the rocket is firing
		double pushForce = 0;
		if (engineTime > 0) pushForce = enginePower;
		else if (sustainerTime > 0) pushForce = sustainerPower;

		// a = F / m
		double accelerationX = (pushForce - drag) / mass;
		speedX += accelerationX * dt;
	}

	void updateYSpeed(double liftCoefficient) {

		double airDensity = calculateAirDensity(altitude);
		double lift = calculateLift(liftCoefficient, airDensity, speedX	, liftArea);
		double gravityForce = calculateGravityForce(mass);

		/*speedY += (lift - gravityForce) / mass;*/
		speedY = 0;

	}

	void updateParameters() {

		altitude += speedY;
		distance += speedX;

		machNumber = speedX / calculateMachSpeed(altitude);

		temperature = calculateTemp(altitude);
		pressure = calculatePressure(altitude);

	}

	void move(float dt) {

		double dragCoefficient = machNumber >= 1 ? 0.2 : 0.5;
		double liftCoefficient = machNumber >= 1 ? 0.2 : 0.15;

		updateXSpeed(dragCoefficient,dt);
		updateYSpeed(liftCoefficient);	
		updateParameters();

		directionX += speedX * dt;
		directionY += speedY * dt;

		directionX = directionX > WORLD_WIDTH ? directionX - WORLD_WIDTH : directionX;
		directionY = directionY > WORLD_HEIGHT ? directionY - WORLD_HEIGHT : directionY;

		if (engineTime > 0) engineTime -= dt;
		else if (sustainerTime > 0) sustainerTime -= dt;

		std::cout << "--- MISSILE TELEMETRY ---" << "\n"
			<< "Phase:    " << (engineTime > 0 ? "BOOST" : sustainerTime > 0 ? "SUSTAIN" : "GLIDE") << "\n"
			<< "Speed X:  " << std::fixed << std::setprecision(2) << speedX << " m/s (" << (speedX * 3.6) << " km/h)\n"
			<< "Speed Y:  " << speedY << " m/s\n"
			<< "Mach:     " << std::setprecision(3) << machNumber << "\n"
			<< "Altitude: " << (int)altitude << " m\n"
			<< "-------------------------" << std::endl;

	}

};

int main() {
	
	Window window;

	Missile AIM7(
		0,        // directionX (Position)
		5000,        // directionY (Position)
		1000,      // speedX (Inherited from the plane)
		0.0f,          // speedY
		17800.0,       // enginePower (Boost)
		2.8,           // engineTime
		4500.0,        // sustainerPower (Sustain)
		11.2,          // sustainerTime
		0.032,         // dragArea
		0.45,          // liftArea
		231.0          // mass (newtons)
	);

	GLfloat point[] = { AIM7.directionX,AIM7.directionY };
	//
	Shader shaderProgram("default.vert", "default.frag");
	//
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(point, sizeof(point));

	VAO1.LinkAttrib(VBO1, 0, 2, GL_FLOAT, GL_FALSE, (void*)0);

	VAO1.Unbind();
	VBO1.Unbind();
	//
	glPointSize(25.0f);

	double lastFrame = 0;

	while (!glfwWindowShouldClose(window.window)) {

		double currentFrame = glfwGetTime();
		double deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		AIM7.move(deltaTime);

		VBO1.Bind();

		GLfloat updated[] = { AIM7.directionX,AIM7.directionY };
		
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(updated), updated);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();

		glm::mat4 projection = glm::ortho(0.0f, WORLD_WIDTH, 0.0f, WORLD_HEIGHT, -1.0f, 1.0f);

		// Send this matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		VAO1.Bind();
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window.window);
		glfwPollEvents();

	}
}
