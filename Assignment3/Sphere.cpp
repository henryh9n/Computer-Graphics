//
//  Sphere.cpp
//  Assignment3
//
//  Created by Henry Harutyunyan on 4/16/19.
//  Copyright © 2019 hharutyunyan. All rights reserved.
//

#include "Sphere.hpp"

#include <iostream>
using namespace std;

Sphere::Sphere(float radius, int stacks, int sectors)
    : radius(radius), stacks(stacks), sectors(sectors)
{
    genSphere();
}


GLfloat * Sphere::getVertices()
{
    return &vertices[0];
}

GLuint * Sphere::getIndices()
{
    return &indices[0];
}

void Sphere::genSpherePole(int pole)
{
    vertices.push_back(0.0f);
    vertices.push_back(pole * radius);
    vertices.push_back(0.0f);
    
//    Vertex normal
    vertices.push_back(0.0f);
    vertices.push_back(pole);
    vertices.push_back(0.0f);
    
    GLfloat y = pole * radius * sin(M_PI/2 - phi);
    float pref = radius * cos(M_PI/2 - phi);
    
    for (int i = 0; i < sectors; i++)
    {
        vertices.push_back(pref * sin(theta * i));
        vertices.push_back(y);
        vertices.push_back(pref * cos(theta * i));
        
//        Adding normals
        vertices.push_back(pref * sin(theta * i) / radius);
        vertices.push_back(y / radius);
        vertices.push_back(pref * cos(theta * i) / radius);
    }
    
//    Redefining the first vertex for complete circle
    vertices.push_back(0);
    vertices.push_back(y);
    vertices.push_back(pref);
    
//        Adding normals
    vertices.push_back(0);
    vertices.push_back(y / radius);
    vertices.push_back(pref / radius);
}

void Sphere::genSphereBody()
{
    genSpherePole();
    
    for (int i = 1; i < stacks; i++)
    {
        float c_phi = (M_PI/2) - (phi * i);
        GLfloat y = radius * sin(c_phi);
        
        for (int e = 0; e < sectors; e++)
        {
            float pref = radius * cos(c_phi);
            
            vertices.push_back(pref * sin(theta * e));
            vertices.push_back(y);
            vertices.push_back(pref * cos(theta * e));
            
//            Adding normals
            vertices.push_back(pref * sin(theta * e) / radius);
            vertices.push_back(y / radius);
            vertices.push_back(pref * cos(theta * e) / radius);
        }
    }
    
    genSpherePole(-1);
}

void Sphere::genIndices()
{
    for (int i = 1; i < stacks - 1; i++)
    {
        for (int e = 1; e <= sectors; e++)
        {
            indices.push_back(i * sectors + e + 1);
            indices.push_back((i + 1) * sectors + e + 1);
        }
        indices.push_back(i * sectors + 2);
        indices.push_back((i + 1) * sectors + 2);
    }
}

long Sphere::getVerticesCount()
{
    return vertices.size();
}

long Sphere::getIndicesCount()
{
    return indices.size();
}
