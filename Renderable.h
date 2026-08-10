// Renderable.h
#pragma once
#include <glad/glad.h>
#include "VAO.h"
#include "VBO.h"

class Renderable {
public:
    Renderable(GLfloat nose_aoa, GLfloat direction_x, GLfloat direction_y);

    void Update_Pos(GLfloat nose_aoa, GLfloat x, GLfloat y);          // pushes new position into the VBO
    void Draw();                                // binds VAO and issues the draw call

private:
    GLfloat nose_aoa = 0.0f;
    GLfloat point[2] = {};
    VAO vao;
    VBO vbo;
};