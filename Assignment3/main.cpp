#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Sphere.hpp"
#include "Shader.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

// Buffers
GLuint VBO, VAO, IBO;


// Global vars for radius of the sphere,
// number of its stacks and sectors
float radius = 0.8;
unsigned int stacks = 20, sectors = 20;


// Globals for mouse position
float mouseX;
float angleX = 0;

glm::vec3 cameraPostion (0.0f, 0.0f, 1.0f);
glm::vec3 lightPos (0.0f, 5.0f, 0.0f);


// Functions used in the program
void getParams();
void drawSphere(long indicesCount);
void cleanUp();
void mouseDrag(GLFWwindow * window, double xpos, double ypos);
void mouseClick(GLFWwindow * window, int button, int action, int mods);


int main()
{
//    getParams();
    
//    Initializing GLFW
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
//    Creating the window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "Assignment 3", NULL, NULL);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
//    Checking if window is created succesfully
    if (nullptr == window)
    {
        cout << "Error creating the window.\n";
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
//    Setting the callbacks
    glfwSetCursorPosCallback(window, mouseDrag);
    
//    Initializing GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        cout << "Error initializing the GLFW.\n";
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
    

//    Initializing the shader program
    Shader mainShader;
    
//    Setting Up the sphere and generating its vertices and indices
    Sphere mySphere(radius, stacks, sectors);
    GLfloat * vertices = mySphere.getVertices();
    GLuint * indices = mySphere.getIndices();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
//    Making the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 mySphere.getVerticesCount() * sizeof(GLfloat),
                 vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mySphere.getIndicesCount() * sizeof(GLuint),
                 indices, GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mainShader.implementShaders();
        
        glm::mat4 model = glm::mat4(1.0f);
        
        
        glm::mat4 view = glm::lookAt(cameraPostion,
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        
        view = glm::rotate(view, angleX * 0.7f,
                           glm::vec3(0.0f, 1.0f, 0.0f));

        
        GLfloat proportion = (GLfloat) screenWidth / screenHeight;
        glm::mat4 projection = glm::ortho(-1.0f * proportion,   proportion,
                                          -1.0f,                1.0f,
                                          -1.0f * proportion,   proportion);
        
        GLint modelLoc = glGetUniformLocation(mainShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(mainShader.Program, "view");
        GLint projectionLoc = glGetUniformLocation(mainShader.Program,
                                                   "projection");
        GLint lightLoc = glGetUniformLocation(mainShader.Program, "lightPos");
        
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
                           glm::value_ptr(projection));
        glUniform3fv(lightLoc, 1, glm::value_ptr(lightPos));
        
//        Drawing the sphere
        drawSphere(mySphere.getIndicesCount());
        
        glfwSwapBuffers(window);
    }
    
    cleanUp();
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}


void getParams()
{
    cout << "Please enter the radius of the sphere: ";
    cin >> radius;
    
    cout << "Please enter the number of stacks: ";
    cin >> stacks;
    
    cout << "Please enter the number of sectors: ";
    cin >> sectors;
}


void drawSphere(long indicesCount)
{
    glBindVertexArray(VAO);
    
//    North pole of the sphere using Triangle Fan
    glDrawArrays(GL_TRIANGLE_FAN, 0,  sectors + 2);
    
//    Body of the sphere using Trianlge Strips
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, (GLvoid *) 0);
    
//    South pole of the sphere using Triangle Fan
    glDrawArrays(GL_TRIANGLE_FAN, sectors * stacks + 2,  sectors + 2);
    
    glBindVertexArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void cleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}


void mouseDrag(GLFWwindow * window, double xpos, double ypos)
{
    float xoffset = xpos - mouseX;
    
    mouseX = xpos;

    angleX += 2 * M_PI * xoffset / WINDOW_WIDTH;
}
