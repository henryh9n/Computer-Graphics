//
//  Shader.cpp
//  Assignment3
//
//  Created by Henry Harutyunyan on 4/23/19.
//  Copyright © 2019 hharutyunyan. All rights reserved.
//

#include "Shader.hpp"

using namespace std;

Shader::Shader(string vertexShaderPath, string fragmentShaderPath)
    :   vertexShaderPath(vertexShaderPath),
        fragmentShaderPath(fragmentShaderPath)
{
    ifstream vertexShaderFile (this->vertexShaderPath),
        fragmentShaderFile (this->fragmentShaderPath);
    
    stringstream vertexShaderStream, fragmentShaderStream;
    
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();
    
    vertexShaderFile.close();
    fragmentShaderFile.close();
    
    string vertexShaderSource = vertexShaderStream.str();
    string fragmentShaderSource = fragmentShaderStream.str();
    
    GLint status;
    GLchar info[512];
    
//    Compiling vertex shader
    const GLchar * vertexShaderCode = vertexShaderSource.c_str();
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    
//    Checking for compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, info);
        cout << "Error compiling vertex shader.\n" << info << endl;
    }
    
//    Compiling fragment shader
    const GLchar * fragmentShaderCode = fragmentShaderSource.c_str();
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    
//    Chaching for compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info);
        cout << "Error compiling fragment shader.\n" << info << endl;
    }
    
//    Creating the program
    this->Program = glCreateProgram();
    
    glAttachShader(this->Program, vertexShader);
    glAttachShader(this->Program, fragmentShader);
    glLinkProgram(this->Program);
    
//    Chacking for linking errors
    glGetProgramiv(this->Program, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(this->Program, 521, NULL, info);
        cout << "Error linking shader program.\n" << info << endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::implementShaders()
{
    glUseProgram(this->Program);
}

void Shader::setUniforms(glm::mat4 &model)
{
    string modelName = "model";
    glUniformMatrix4fv(glGetUniformLocation(this->Program, modelName.c_str()), 1, GL_FALSE, &model[0][0]);
}
