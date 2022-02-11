// 3D_MAZE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include<stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Window.h"
#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"Floor.h"
#include"Camera.h"
#include"GenMap.h"
#include"Wall.h"
#include"WallLight.h"


Window mainWindow;
std::vector<Mesh*>meshList;
std::vector<Shader> shaderList;


GenMap map;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, -1.0f);
GLfloat ambientIntensity = 0.1f;
glm::vec3 ambientColor = glm::vec3(246.0f / 255.0f, 228.0f / 255.0f, 188.0f / 255.0f);

//Vertex shader
static const char* vShader = "Shaders/shader.vert";
static const char* vLightingShader = "Shaders/Lightingshader.vert";

//fragment shader
static const char* fShader = "Shaders/shader.frag";
static const char* fLightingShader = "Shaders/Lightingshader.frag";


void CreateShaders(const char* vertShader,const char* fragShader) {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vertShader, fragShader);
    shaderList.push_back(*shader1);
     
}
int main()
{
    
    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();
    WallLight light= WallLight(lightPos.x, lightPos.y, lightPos.z,
        ambientColor.x, ambientColor.y, ambientColor.z,
                                1.0f);
    map = GenMap(20, 10, 0.8f);
    //light = Light(0.5f, 0.5f, 0.0f);

    CreateShaders(vShader, fShader);//shaderList[0]
    CreateShaders(vLightingShader, fLightingShader);//shaderList[1]

    camera = Camera(glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f, 0.0f,0.0f, 5.0f, 0.5f);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    GLfloat i = 0;
    while (!mainWindow.getShouldClose())
	{
        //delta time calculation
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        //input
        glfwPollEvents();
		mainWindow.processInput(deltaTime);
        camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());
        camera.keyControl(mainWindow.getsKeys());


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        shaderList[0].setMat4("projection", projection);
        shaderList[0].setMat4("view", camera.getViewMatrix());
        shaderList[0].setvec3("lightColor", ambientColor);
        shaderList[0].setFloat("ambientIntensity", ambientIntensity);
        shaderList[0].setvec3("lightPos", lightPos);
        map.Draw(shaderList[0]);

        shaderList[1].UseShader();
        shaderList[1].setMat4("projection", projection);
        shaderList[1].setMat4("view", camera.getViewMatrix());
        light.Apply(shaderList[1]);
        glUseProgram(0);

        mainWindow.swapBuffers();
	}
	glfwTerminate();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
