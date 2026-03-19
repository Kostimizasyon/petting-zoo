#include "VBO.h"


//VERTEX BUFFER OBJECT
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{

	glGenBuffers(1, &ID); //we only have 1 object,pointing it to reference

	glBindBuffer(GL_ARRAY_BUFFER, ID); //Telling the "State Machine" what we want to work with ;; this is kinda like myBuffer.setData(data) but since opengl isnt a c++ thing we have to do this:

	//GL_DYNAMIC => verticies will be modified multiple times and used a lot of times
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW); //GL_STREAM =>verticies will be modified once and used a few times
	//_DRAW,_READ,COPY										       //GL_STATIC => verticies will be modified once and used a lot of times

	//We use size, as the verticies passed into this constructor is a pointer, and size of would give the size of that pointer not the verticies itself

}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}