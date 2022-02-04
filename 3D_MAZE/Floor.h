#pragma once
#include <iostream>
#include<stdio.h>
#include<vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"Mesh.h"

class Floor
{
public:
	Floor();

    void Rotate(float RotateX, float RotateY , float RotateZ);
    void Translate(GLfloat transX, GLfloat transY, GLfloat transZ);
    void Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
    void Render();
    void SetModel(GLuint uniformLocation);
    ~Floor();


private:
    unsigned int Floorindices[6] = {
        0,1,3,
        1,2,3
    };

    GLfloat Floorvertices[32]= {
        //  x     y     z       u       v       nx   ny   nz
        0.5f,   0.5f,   0.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,
        0.5f,  -0.5f,   0.0f,   1.0f,   0.0f,   0.0f,   -1.0f,  0.0f,
        -0.5f, -0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   -1.0f,  0.0f,
        -0.5f,  0.5f,   0.0f,   0.0f,   1.0f,   0.0f,   -1.0f,  0.0f,
    };
    Mesh* obj;
    glm::quat quaternion;
    glm::mat4 TransModel;
    glm::mat4 ScaleModel;
};

