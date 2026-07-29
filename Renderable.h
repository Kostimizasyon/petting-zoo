// Renderable.h
#pragma once
#include <glad/glad.h>
#include "VAO.h"
#include "VBO.h"

class Renderable {
public:
    Renderable(GLfloat direction_x, GLfloat direction_y);

    void Update_Pos(GLfloat x, GLfloat y);          // pushes new position into the VBO
    void Draw();                                // binds VAO and issues the draw call

private:
    GLfloat point[2];
    VAO vao;
    VBO vbo;
};