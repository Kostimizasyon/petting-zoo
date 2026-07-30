#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window_constants.h"
#include "ShaderPlus.h"
#include "VAO.h"
#include "VBO.h"

struct Window {
	GLFWwindow* window;
	int WIDTH = window_const::WINDOW_WIDTH;
	int HEIGHT = window_const::WINDOW_HEIGHT;

	VAO* line_VAO;
	VBO* line_VBO;

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

		// placeholder data, overwritten every frame in DrawAngleLine
		GLfloat placeholder[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		line_VBO = new VBO(placeholder, sizeof(placeholder));
		line_VAO = new VAO();

		line_VAO->LinkAttrib(*line_VBO, 0, 2, GL_FLOAT, 2 * sizeof(GLfloat), (void*)0);
	}

	void Render(Shader shader_program) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader_program.Activate();

		glm::mat4 projection = glm::ortho(0.0f, window_const::WORLD_WIDTH, 0.0f, window_const::WORLD_HEIGHT, -1.0f, 1.0f);

		// Send this matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(shader_program.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	// draws a flat line from (x, y) at the given angle (radians), of given length
	void DrawAngleLine(Shader shader_program, float x, float y, float angle, float length = 50.0f) {
		float end_x = x + length * cosf(angle);
		float end_y = y + length * sinf(angle);

		GLfloat vertices[4] = { x, y, end_x, end_y };

		shader_program.Activate();

		glm::mat4 projection = glm::ortho(0.0f, window_const::WORLD_WIDTH, 0.0f, window_const::WORLD_HEIGHT, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader_program.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		line_VBO->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		line_VBO->Unbind();

		line_VAO->Bind();
		glDrawArrays(GL_LINES, 0, 2);
		line_VAO->Unbind();
	}
};