#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"Mesh.h"
#include"Texture.h"
class Wall
{
public:
	Wall();
	
    void Rotate(float RotateX, float RotateY, float RotateZ);
    void Translate(GLfloat transX, GLfloat transY, GLfloat transZ);
    void Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
    void Draw(GLuint uniformLocation);

    ~Wall();
private:
    unsigned int Wallindices[6] = {
        0,1,3,
        1,2,3
    };

    GLfloat Wallvertices[32] = {
        //  x     y     z           u       v       nx      ny      nz
        0.5f,   0.4f,   0.0f,     5.0,   5.0,   0.0f,   -1.0f,  0.0f,
        0.5f,   -0.0f,   0.0f,    5.0,   0.0f,   0.0f,   -1.0f,  0.0f,
        0.0f,  -0.0f,   0.0f,    0.0f,   0.0f,   0.0f,   -1.0f,  0.0f,
        0.0f,  0.4f,   0.0f,     0.0f,   5.0,   0.0f,   -1.0f,  0.0f,
    };
    Mesh* obj;
    Texture* texture;
    glm::quat quaternion;
    glm::mat4 TransModel;
    glm::mat4 ScaleModel;
    void SetTexture();
    void SetModel(GLuint uniformLocation);
};

