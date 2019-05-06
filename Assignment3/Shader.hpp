//
//  Shader.hpp
//  Assignment3
//
//  Created by Henry Harutyunyan on 4/23/19.
//  Copyright © 2019 hharutyunyan. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;


class Shader
{
public:
    GLuint Program;
    
    Shader(string vertexShaderPath = "Shaders/vertex.shader",
           string fragmentShaderPath = "Shaders/fragment.shader");
    
    void implementShaders();
    void setUniforms(glm::mat4 &model);
private:
    string vertexShaderPath;
    string fragmentShaderPath;
};


#endif /* Shader_hpp */
