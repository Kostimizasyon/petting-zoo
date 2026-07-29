#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window_constants.h"
#include "ShaderPlus.h"

struct Window {
	GLFWwindow* window;
	int WIDTH = window_const::WINDOW_WIDTH;
	int HEIGHT = window_const::WINDOW_HEIGHT;

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

	void Render(Shader shader_program) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader_program.Activate();

		glm::mat4 projection = glm::ortho(0.0f, window_const::WORLD_WIDTH, 0.0f, window_const::WORLD_HEIGHT, -1.0f, 1.0f);

		// Send this matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(shader_program.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}
};