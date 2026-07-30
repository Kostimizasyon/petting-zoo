// Renderable.h
#pragma once
#include <glad/glad.h>
#include "VAO.h"
#include "VBO.h"

class Renderable {
public:
    Renderable(GLfloat angle_of_attack, GLfloat direction_x, GLfloat direction_y);

    void Update_Pos(GLfloat angle_of_attack, GLfloat x, GLfloat y);          // pushes new position into the VBO
    void Draw();                                // binds VAO and issues the draw call

private:
    GLfloat angle_of_attack = 0.0f;
    GLfloat point[2] = {};
    VAO vao;
    VBO vbo;
};