// Renderable.cpp
#include "Renderable.h"

Renderable::Renderable(GLfloat x, GLfloat y)
    : point{ x, y },
    vbo(point, sizeof(point))   // VBO ctor uploads initial data
{
    vao.Bind();
    vbo.Bind();

    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, GL_FALSE, (void*)0);

    vao.Unbind();
    vbo.Unbind();
}

void Renderable::Update_Pos(GLfloat x, GLfloat y) {    
    point[0] = x;
    point[1] = y;

    vbo.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point), point);
    vbo.Unbind();
}

void Renderable::Draw() {
    vao.Bind();
    glDrawArrays(GL_POINTS, 0, 1);
    vao.Unbind();
}