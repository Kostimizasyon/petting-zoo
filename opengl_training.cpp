#include "main.h"

GLfloat vertices[] = {
	//------COORDS-----        ----COLORS----    ---COORDS FOR IMAGE--
	-1.0f,  1.0f, 0.0f,        1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		// Top Left(Red)
	 1.0f,  1.0f, 0.0f,        1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// Top Right (Red)
	 1.0f, -1.0f, 0.0f,        1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		// Bottom Right (Red)
	-1.0f, -1.0f, 0.0f,        1.0f, 0.0f, 0.0f,	0.0f, 0.0f		// Bottom Left (Red)
};

//The order in which we want OpenGL to go over the verticies:
GLuint indices[] = {
	0,2,1, 
	0,3,2
};

int main() 
{
	glfwInit();

		//Since glfw aint the smartets around we need to give it some hints as to what we are using
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //We are telling GLFW what major version of opengl we are using
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //We are telling GLFW what minor version of opengl we are using
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We are telling GLFW what profile of opengl we are using
		
		GLFWwindow* window = glfwCreateWindow(800,800,"ASAP ROCKY", NULL, NULL); //We are creating the window => it will be 800x800, named ASAP ROCKY, wont be full screen,
		
		//Null check
		if (window == NULL) {
			std::cout << "Window failed to initialize" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window); //Telling the dumbass that yes we will use it.


		gladLoadGL(); //telling glad to initialize


		glViewport(0,0,800,800); //Telling opengl how much of the window it should render => we selected bottom right to top right so the entire thing


		//---------------------------------------------
		Shader shaderProgram("default.vert", "default.frag");
		//---------------------------------------------
		VAO VAO1;
		VAO1.Bind();

		VBO VBO1(vertices, sizeof(vertices));
		EBO EBO1(indices, sizeof(indices));
		
		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);					  //our aVec passing to the vert shader,
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); //our aColor passing to the vert shader
		VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); //our aTex passing to the vert shader
		
		VBO1.Unbind();
		VAO1.Unbind();
		EBO1.Unbind();

		GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); //we say that uniID will point to the scale, we need shaderProgram to be initialized to do so

		//---------------------------------------------

		int widthImg, heightImg, numColCh;
		
		stbi_set_flip_vertically_on_load(true);

		unsigned char* bytes = stbi_load("LoopThingy.png", &widthImg, &heightImg, &numColCh, 0);

		Texture texture(widthImg,heightImg,bytes);


		stbi_image_free(bytes); //free the image
		texture.Unbind();

		GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
		shaderProgram.Activate();
		glUniform1i(tex0Uni, 0);

		//---------------------------------------------
		
		/*
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGB + opacity
		glClear(GL_COLOR_BUFFER_BIT); //Clear the backbuffer
		glfwSwapBuffers(window); //We tell opengl to switch to the backbuffer as we updated it
		*/
		

		while (!glfwWindowShouldClose(window)) // if we dont put the program in an infinite loop, it will terminate itself when it reaches the end, so we do a infinite loop
		{	
			//Specify the color of the background
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			//Clear the back buffor and assign new color
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.Activate(); //tell opengl which shader program we want
			glUniform1f(uniID, 0.75f); //we set the uniform scale to 0.75f

			glBindTexture(GL_TEXTURE_2D, texture.ID);

			VAO1.Bind();

			//glDrawArrays(GL_TRIANGLES, 0, 3); //what primative we wnat to use,which index our vertex is in,amount of vertecies we wnat to draw
			
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //primative,amount of the indicies, data type of indicies, index of our indicies
			
			glfwSwapBuffers(window); //Image gets updated each frame

			
									// where it will last untill either we close the window or someone else (a function) asks the window to close
			glfwPollEvents();		//Process when the window appears,resized etc etc => if we wont process them it wont respond
		}

		glfwDestroyWindow(window);

		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();
		texture.Delete();
		shaderProgram.Delete();


	glfwTerminate();

	return 0;
}