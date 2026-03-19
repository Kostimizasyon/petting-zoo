#include "ShaderPlus.h"

static std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) 
{
    
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //GL version of unsigned int

    glShaderSource(vertexShader, 1, &vertexSource, NULL);  //Feeding the source code to the shader => we give it the reference value,1 string for whole shader,reference to source code
    glCompileShader(vertexShader); //GPU cant understand our source code that is in c++ so we need to compile it to machine code
    CompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    CompileErrors(fragmentShader, "FRAGMENT");

    ID = glCreateProgram(); //to use the shaders we need to create the program to 

    //giving the shaderProgram our shaders
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    
    CompileErrors(ID, "PROGRAM");
    
    //Our shaders are already in the shader program itself so we no longer need them
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);
}

void Shader::Activate() 
{
    glUseProgram(ID);
}

void Shader::Delete() 
{
    glDeleteProgram(ID);
}

void Shader::CompileErrors(unsigned int shader, const char* type) {

    GLint hasCompiled;
    char infoLog[1024];

    if (type != "PROGRAM") {

        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER COMPILATION ERROR for: " << type << "\n";
        }

    }
    else {

        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);

        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER COMPILATION ERROR for: " << type << "\n";
        }

    }
}