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
#include"Texture.h"

class Floor
{
public:
	Floor();
    Floor(GLuint xNum, GLuint yNum);

    void Rotate(float RotateX, float RotateY , float RotateZ);
    void Translate(GLfloat transX, GLfloat transY, GLfloat transZ);
    void Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
    void Draw(GLuint uniformLocation);
    //void Render();
    
    ~Floor();


private:
    unsigned int Floorindices[6] = {
        0,1,3,
        1,2,3
    };

    GLfloat Floorvertices[32]= {
        //  x     y     z       u       v       nx   ny   nz
        0.25f,   0.0f,   0.25f,   20.0,   20.0,   0.0f,   -1.0f,  0.0f,
        0.25f,   0.0f,   -0.25f,   20.0,   0.0f,   0.0f,   -1.0f,  0.0f,
        -0.25f,  0.0f,   -0.25f,   0.0f,   0.0f,   0.0f,   -1.0f,  0.0f,
        -0.25f,  0.0f,   0.25f,   0.0f,   20.0,   0.0f,   -1.0f,  0.0f,
    };
    Mesh* obj;
    Texture* texture;
    std::vector<glm::vec3> floorPositions;
    GLuint xNum, yNum;
    glm::quat quaternion;
    glm::mat4 TransModel;
    glm::mat4 ScaleModel;
    void SetTexture();
    void SetModel(GLuint uniformLocation);
};

