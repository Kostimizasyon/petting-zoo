#pragma once
#include "ShaderPlus.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"
#include <iostream>
#include <math.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>                  // Basic math (vec3, mat4)
#include <glm/gtc/matrix_transform.hpp> // Matrix math (translate, rotate, ortho)
#include <glm/gtc/type_ptr.hpp>        // Converting matrices to OpenGL format
#include <iomanip>

#include <Window.h>

// Actual shit

#include "AeroDynamics.h"
#include "AerialObject.h"
#include "Missile.h"
#include "Aircraft.h"