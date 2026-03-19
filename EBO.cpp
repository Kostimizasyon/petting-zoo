#include "EBO.h"


//ELEMENTS BUFFER OBJECT
EBO::EBO(GLuint* indices, GLsizeiptr size)
{

	glGenBuffers(1, &ID); //we only have 1 object,pointing it to reference

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); //Telling the "State Machine" what we want to work with ;; this is kinda like myBuffer.setData(data) but since opengl isnt a c++ thing we have to do this:

	//GL_DYNAMIC => verticies will be modified multiple times and used a lot of times
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); //GL_STREAM =>verticies will be modified once and used a few times
	//_DRAW,_READ,COPY													       //GL_STATIC => verticies will be modified once and used a lot of times


}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}