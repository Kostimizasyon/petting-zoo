#include "VAO.h"

//VERTEX ARRAY OBJECT

VAO::VAO() 
{
	glGenVertexArrays(1, &ID);
	
}

void VAO::LinkAttrib(VBO VBO,GLuint layout,GLuint numComponents,GLenum type, GLsizeiptr stride,void* offset )
{
	VBO.Bind();
	glBindVertexArray(ID);

	//We send the shader a lot of bytes, then we tell openGL how to handle it:
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//layout is the position of our vertex attribute, our num components is the how many values we have per vertex, our type is the data type we use in the vertex
	//GL_FALSE ignore, stride is the gap between brothers , and offset is the gap between brothers without including itself 


	glEnableVertexAttribArray(layout); //position of vertex attribute
	VBO.Unbind(); //Ordering matters
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}