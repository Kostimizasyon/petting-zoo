#include "main.h"
#define GAMMA_CONSTANT 1.4
#define GAS_CONSTANT 8.31446
#define GRAVITY_CONSTANT -9.81f
#define TEMPERATURE 293

struct Window {
	GLFWwindow* window;
	int WIDTH = 1920;
	int HEIGHT = 1080;
	
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


	double calculateMach(double altitude) {

		return (GAMMA_CONSTANT * GAS_CONSTANT * calculateTemp(altitude));

	}

	double calculateTemp(double altitude) {
		//Temperature drops 1.98K per 1000 feet
		return (TEMPERATURE - (1.98 / 1000 * altitude));

	}

	//Atmospheric pressure = P0(sea level pressure) * ( 1 - (L (temperature lapse per m) * h / T0) ** - g * M (molar mass of dry air) / gas constant * L

	double calculatePressure(double altitude) {

		return pow((101.325 * (1 + (-1.98 / 1000 * altitude) / TEMPERATURE)), -(GRAVITY_CONSTANT * 0.02896968 / GAS_CONSTANT * -0.00976));

	}

	//Air density : pM / RT => atmosphericPressure * molar mass of gas / Gas Constant * Temperature
	double calculateAirDensity(double pressure, double altitude, double temperature) {

		return (pressure * 0.02896968) / GAS_CONSTANT * temperature;

	}


};

class Missile :AeroDynamics {
public:
	//Position
	GLfloat directionX;
	GLfloat directionY;

	//Info
	GLfloat speedX;
	GLfloat speedY;
	double altitude;
	double distance;
	double machNumber;

	//Engine info
	double enginePower;
	double engineTime;

	//Sustainer info
	double sustainerPower;
	double sustainerTime;

	//Misc
	double dragArea;
	double liftArea;
	double mass;
	double temperature;
	double pressure;

	Missile(GLfloat directionX, GLfloat directionY, GLfloat speedX, GLfloat speedY, double enginePower, double engineTime, double sustainerPower, double sustainerTime, double dragArea, double liftArea, double mass) 
	{
		
		altitude = directionY;

		machNumber = speedX / calculateMach(altitude);

		temperature = calculateTemp(altitude);
		pressure = calculatePressure(altitude);
	
	}

	void updateXSpeed(double dragCoefficient) {
		//Check how much thrusth we have										Calculate amount of drag
		speedX += (engineTime > 0 ? enginePower : sustainerTime > 0 ? sustainerPower : 0) - calculateDrag(dragCoefficient, calculateAirDensity(pressure, altitude, temperature), speedX, dragArea);

	}

	void updateYSpeed(double liftCoefficient) {

		speedY += calculateLift(liftCoefficient, calculateAirDensity(pressure, altitude, temperature), speedY, liftArea) - calculateGravityForce(mass);

	}

	void updateParameters() {

		altitude += speedY;
		distance += speedX;

		machNumber = speedX / calculateMach(altitude);

		temperature = calculateTemp(altitude);
		pressure = calculatePressure(altitude);

	}

	void move() {

		double dragCoefficient = machNumber >= 1 ? 0.2 : 0.5;
		double liftCoefficient = machNumber >= 1 ? 0.2 : 0.15;

		updateXSpeed(dragCoefficient);
		updateYSpeed(liftCoefficient);
		updateParameters();

		directionX += speedX;
		directionY += speedY;

	}

};

int main() {
	
	Window window;

	Missile AIM7(0, 20000, 485.9, 0, 50000, 2.5, 7000, 7.0, 0.034, 0.180, 231);

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
	glPointSize(2.0f);

	double lastFrame = 0.0;

	while (!glfwWindowShouldClose(window.window)) {

		double currentFrame = glfwGetTime();
		double deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		AIM7.move();

		VBO1.Bind();

		GLfloat updated[] = { AIM7.directionX,AIM7.directionY };
		
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(updated), updated);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window.window);
		glfwPollEvents();

	}
}

struct Star {

	GLfloat posx, posy;
	GLfloat dirx, diry;

	GLfloat MASS;

	Star(GLfloat posx, GLfloat posy, GLfloat dirx, GLfloat diry, GLfloat MASS) : posx(posx), posy(posy), dirx(dirx), diry(diry), MASS(MASS) {};

	void acceleratedMove(float deltaTime) {
		// F = ma  ->  a = F/m
		GLfloat starXAcceleration = (100 / MASS) / 300;
		GLfloat starYAcceleration = (GRAVITY_CONSTANT / 100);

		// v = v + a * dt
		dirx += starXAcceleration * deltaTime;
		diry += starYAcceleration * deltaTime;

		// p = p + v * dt
		posx += dirx * deltaTime;
		posy += diry * deltaTime;
	}

};

