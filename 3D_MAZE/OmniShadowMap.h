#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<vector>
class OmniShadowMap
   
{
public:
    OmniShadowMap();

    bool Init(GLuint width, GLuint height);

    void Write();

    void Read(GLenum textureUnit);
    GLuint GetShadowWidth() { return shadowWidth; }
    GLuint GetShadowHeight() { return shadowHeight; }
    ~OmniShadowMap();
protected:
    GLuint FBO, depthCubeMap;
    GLuint shadowWidth, shadowHeight;
};