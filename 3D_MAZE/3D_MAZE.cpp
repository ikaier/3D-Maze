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
#include"MyTransform.h"
#include"Floor.h"
#include"Camera.h"


Window mainWindow;
std::vector<Mesh*>meshList;
std::vector<Shader> shaderList;


Texture brickTexture;
MyTransform demoTransform;
Camera camera;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//Vertex shader
static const char* vShader = "Shaders/shader.vert";

//fragment shader
static const char* fShader = "Shaders/shader.frag";


void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
     
}
int main()
{
    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();

    brickTexture = Texture("Textures/brick.jpg");
    brickTexture.LoadTexture();

    Floor anFloor;
    //anFloor.Rotate(0.0.0f, 0.0f, 0.0f);

    CreateShaders();
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f, 0.0f,0.0f, 5.0f, 0.5f);

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    GLfloat i = 0;
    while (!mainWindow.getShouldClose())
	{
        GLfloat now = glfwGetTime();//SDL_GetPerformaceCounter();
        deltaTime = now - lastTime;//(now-lastTime)*1000/SDL_GetPerformaceFrequency()
        lastTime = now;
        glfwPollEvents();
		mainWindow.processInput(deltaTime);
        camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());

        camera.keyControl(mainWindow.getsKeys());
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        brickTexture.UseTexture();
        shaderList[0].UseShader();
        

        anFloor.SetModel(shaderList[0].GetTransformLocation());
        
        glm::mat4 view = camera.getViewMatrix();
        
        glUniformMatrix4fv(shaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
        
        anFloor.Render();
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
