#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>

#include"Mesh.h"
#include"Texture.h"
class Wall
{
public:
	Wall();
    Wall(std::vector<glm::vec3> walls,GLuint WallCount,GLfloat gridSize);
	
    void Rotate(float yAngle);
    void Translate(GLfloat transX, GLfloat transY, GLfloat transZ);
    void Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
    void Draw();
    void Set();

    ~Wall();
private:
    std::vector<glm::vec3> walls;

    GLuint WallCount;
    GLfloat gridSize;

    GLuint count;
    unsigned int WallIndices[36] = {
        //front and back
         0,1,2,
         0,2,3,

         6,5,4,
         7,6,4,

         //left and right
         9,13,14,
         14,10,9,

        15,12,8,
         8,11,15,

         //top and buttom
         21,17,16,
         16,20,21,

         19,18,22,
         22,23,19
    };

    GLfloat WallVertices[192] = {
        //  x     y    z    u   v        nx   ny   nz
        //front
        0.0f,0.0f,0.0f,   0.0f,0.0f,   0.0f,0.0f,-1.0f,               //left bottom 0
        1.0f,0.0f,0.0f,    2.0f,0.0f,   0.0f,0.0f,-1.0f,               //right bottom 1
        1.0f,1.0f,0.0f,     2.0f,2.0f,   0.0f,0.0f,-1.0f,               //right top 2
        0.0f,1.0f,0.0f,    0.0f,2.0f,   0.0f,0.0f,-1.0f,               //left top 3

        //back
        0.0f,0.0f,0.01f,   0.0f,0.0f,   0.0f,0.0f,1.0f,                // 4
        1.0f,0.0f,0.01f,    2.0f,0.0f,   0.0f,0.0f,1.0f,                // 5
        1.0f,1.0f,0.01f,     2.0f,2.0f,   0.0f,0.0f,1.0f,                // 6
        0.0f,1.0f,0.01f,    0.0f,2.0f,   0.0f,0.0f,1.0f,                //7

        //left
        0.0f,0.0f,0.0f,     0.02f,0.0f,   -1.0f,0.0f,0.0f,               //left bottom 8
        1.0f,0.0f,0.0f,     0.0f,0.0f,   1.0f,0.0f,0.0f,               //right bottom 9
        1.0f,1.0f,0.0f,      0.02f,2.0f,   1.0f,0.0f,0.0f,               //right top 10
        0.0f,1.0f,0.0f,     0.0f,2.0f,   -1.0f,0.0f,0.0f,               //left top 11

        //right
        0.0f,0.0f,0.01f,   0.0f,0.0f,   -1.0f,0.0f,0.0f,                // 12
        1.0f,0.0f,0.01f,    0.02f,0.0f,   1.0f,0.0f,0.0f,                // 13
        1.0f,1.0f,0.01f,     0.02f,2.0f,   1.0f,0.0f,0.0f,                // 14
        0.0f,1.0f,0.01f,    0.0f,2.0f,   -1.0f,0.0f,0.0f,                //15

        //top
        0.0f,0.0f,0.0f,  0.0f,0.02f,   0.0f,-1.0f,0.0f,                //left bottom 16
        1.0f,0.0f,0.0f,    2.0f,0.02f,   0.0f,-1.0f,0.0f,                //right bottom 17
        1.0f,1.0f,0.0f,     2.0f,0.0f,   0.0f,1.0f,0.0f,                //right top 18
        0.0f,1.0f,0.0f,    0.0f,0.0f,   0.0f,1.0f,0.0f,                //left top 19

        //bot
        0.0f,0.0f,0.01f,   0.0f,0.0f,   0.0f,-1.0f,0.0f,                // 20
        1.0f,0.0f,0.01f,    2.0f,0.0f,   0.0f,-1.0f,0.0f,                // 21
        1.0f,1.0f,0.01f,     2.0f,0.02f,   0.0f,1.0f,0.0f,                // 22
        0.0f,1.0f,0.01f,    0.0f,0.02f,   0.0f,1.0f,0.0f                 //23
    };
    Mesh obj;
    Texture* texture;
    std::vector<glm::mat4>modelMatrics;
    glm::mat4 RotateModel;
    glm::mat4 TransModel;
    glm::mat4 ScaleModel;
    void SetTexture();
    void SetModel();
    void SendModel();
};

