//
//  Sphere.hpp
//  Assignment3
//
//  Created by Henry Harutyunyan on 4/16/19.
//  Copyright © 2019 hharutyunyan. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <vector>
#include <math.h>

using namespace std;

class Sphere
{
public:
    Sphere(float radius, int stacks, int sectors);
    GLfloat * getVertices();
    GLuint * getIndices();
    long getVerticesCount();
    long getIndicesCount();
private:
    GLfloat radius;
    GLuint stacks, sectors;
    vector<GLfloat> vertices;
    vector<GLuint> indices;
    
    GLfloat theta = 2 * M_PI / sectors;
    GLfloat phi = M_PI / stacks;
    
    void genSpherePole(int pole = 1);
    void genSphereBody();
    void genIndices();
    void genSphere()
    {
        genSphereBody();
        genIndices();
    };
};


#endif /* Sphere_hpp */
