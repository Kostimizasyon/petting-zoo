#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
//ALL OPENGL OBJECTS ARE ONLY ACCESABLE BY REFERENCE

// 1. Vertex Shader: Handles the position of each vertex
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// 2. Fragment Shader: Handles the color of the pixels
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit();

	//Since glfw aint the smartets around we need to give it some hints as to what we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //We are telling GLFW what major version of opengl we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //We are telling GLFW what minor version of opengl we are using
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We are telling GLFW what profile of opengl we are using

	/*

	//Safer than normal float for opengl
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f, //x axis, left most = -1 middle = 0 right most = 1
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	*/

	GLfloat vertices[] = {
		 0.0f,  1.0f, 0.0f,
		-0.5f,  0.0f, 0.0f,
		 0.5f,  0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	//The order in which we want OpenGL to go over the verticies:
	GLuint indices[] = {
		0, 1, 2,  // Top Triangle
		1, 3, 4,  // Bottom-Left Triangle
		2, 4, 5   // Bottom-Right Triangle
	};


	GLFWwindow* window = glfwCreateWindow(800, 800, "ASAP ROCKY", NULL, NULL); //We are creating the window => it will be 800x800, named ASAP ROCKY, wont be full screen,

	//Null check
	if (window == NULL) {
		std::cout << "Window failed to initialize" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //Telling the dumbass that yes we will use it.


	gladLoadGL(); //telling glad to initialize


	glViewport(0, 0, 800, 800); //Telling opengl how much of the window it should render => we selected bottom right to top right so the entire thing

	//----------------------------------------------------------------------------------------------

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //GL version of unsigned int

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  //Feeding the source code to the shader => we give it the reference value,1 string for whole shader,reference to source code
	glCompileShader(vertexShader); //GPU cant understand our source code that is in c++ so we need to compile it to machine code

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram(); //to use the shaders we need to create the program to 

	//giving the shaderProgram our shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader); //Our shaders are already in the shader program itself so we no longer need them
	glDeleteShader(fragmentShader);

	//----------------------------------------------------------------------------------------------

		//Vertex Array Object  //OPENGL doesnt even know where to find our vertex buffer, so we create a VAO which holds pointers to multiple VBO's and tells OPENGL how to handle them
		//Vertex Buffer Object //as GPUs are quite slow, we will send our data in large amounts at once using buffers (mostly they are arrays but we only have 1 vertice)
	GLuint VAO, VBO;

	GLuint EBO; //Elements Buffer Object

	glGenVertexArrays(1, &VAO); //MAKE SURE TO CREATE IT BEFORE VBO


	//BINDING => Telling the "State Machine" what we want to work with ;; this is kinda like myBuffer.setData(data) but since opengl isnt a c++ thing we have to do this:

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);//we only have 1 object,pointing it to reference

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_DYNAMIC => verticies will be modified multiple times and used a lot of times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_STREAM =>verticies will be modified once and used a few times
	//_DRAW,_READ,COPY													       //GL_STATIC => verticies will be modified once and used a lot of times


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Ability to communicate with a vertex shard from outside
	// 0 => position of vertex attribute GL_FLOAT => coord data type			 3*sizeof(float) => amount of data per vertex
	// 3 => how many values per vertex   GL_FALSE => only matters if coords int  (void*)0 => pointer to where our vertexes begin at the array (since ours is at the start)

	glEnableVertexAttribArray(0); //position of vertex attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Ordering matters
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//----------------------------------------------------------------------------------------------

	int widthImg, heightImg, numColCh;

	unsigned char* bytes = stbi_load("LoopThingy.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture); //how many textures we want to create, and a reference to the texture
	glActiveTexture(GL_TEXTURE0); //Slide our texture into the 0th index in our texture unit
	glBindTexture(GL_TEXTURE_2D, texture); //We bind out texture, giving its type and reference value

	//Our texture type, the setting we want to modify, and the value we want to give the setting

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //We say that we want to keep pixels as is for magnifiying and minifiying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //We say that both on the s and t axis (x,y) we want the iamge to repeat to wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//Now we generate the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	//type of texture, 0, type of color channels we want the texture to have, width, height, 0, type of color channels of our image, data type of pixels, image data itself 

	//GL_RGB => JPEG ;; GL_RGBA => PNG

	//----------------------------------------------------------------------------------------------

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGB + opacity
	glClear(GL_COLOR_BUFFER_BIT); //Clear the backbuffer
	glfwSwapBuffers(window); //We tell opengl to switch to the backbuffer as we updated it



	while (!glfwWindowShouldClose(window)) // if we dont put the program in an infinite loop, it will terminate itself when it reaches the end, so we do a infinite loop
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 3); //what primative we wnat to use,which index our vertex is in,amount of vertecies we wnat to draw

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0); //primative,amount of the indicies, data type of indicies, index of our indicies

		glfwSwapBuffers(window); //Image gets updated each frame


		// where it will last untill either we close the window or someone else (a function) asks the window to close
		glfwPollEvents();		//Process when the window appears,resized etc etc => if we wont process them it wont respond
	}
	glfwDestroyWindow(window);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}