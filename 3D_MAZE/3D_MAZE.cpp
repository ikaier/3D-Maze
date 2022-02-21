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
#include"Wall.h"
#include"WallLight.h"
#include"MazeMap.h"
#include"Material.h"


Window mainWindow;
Shader mainShader;
Shader lightingCubeShader;
Shader flashShadowShader;
Floor mazeFloor;
Wall mazeWall;
MazeMap mazeMap;
WallLight mazeWallLight;
FlashLight flashLight;
Material wallMaterial;
Material floorMaterial;

GLuint mazeWidth=10, mazeHeight=10;
GLfloat gridSize=0.8f;


Camera camera;
//flashlight cookie texture
Texture lighttexure;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool flashIsOn = false;
bool lPressed = false;

void RenderScene(const Shader &shader) {
    //render floor
    floorMaterial.UseMaterial(shader);
    mazeFloor.Draw(shader.GetTransformLocation());

    //render wall
    wallMaterial.UseMaterial(shader);
    mazeWall.Draw(shader.GetTransformLocation());

}

void RenderLightCube(const Shader& Lightingshader) {
    //mazeWallLight.Draw(Lightingshader.GetTransformLocation());
}

int main()
{
    
    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();

    
    mainShader.CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    lightingCubeShader.CreateFromFiles("Shaders/Lightingshader.vert", "Shaders/Lightingshader.frag");
    flashShadowShader.CreateFromFiles("Shaders/FlashShadowShader.vert", "Shaders/FlashShadowShader.frag");
    
    mazeMap = MazeMap(10, 10, 0.8f);
    mazeFloor = Floor(mazeWidth, mazeHeight, gridSize);
    mazeWall = Wall(mazeMap.GetWalls(),mazeMap.GetWallCount(), gridSize);
    mazeWallLight = WallLight(mazeMap.GetWallLights(), mazeMap.GetWallLightCount());
    wallMaterial = Material(0.3f, 3.0f);
    floorMaterial = Material(0.8f, 256.0f);
    camera = Camera(glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f, 0.0f,0.0f, 5.0f, 0.5f);
    flashLight = FlashLight(camera.getCameraPosition(), camera.getCameraDirection());
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

        /************************ render depth scene from light's perspective ***************************/
        flashLight.SetFlashLight(camera.getCameraPosition(), camera.getCameraDirection());
        flashShadowShader.UseShader();
        flashShadowShader.setMat4("lightSpaceMatrix", flashLight.GetLightSpaceMatrix());

        glViewport(0, 0, flashLight.GetShadowMap()->GetShadowHeight(), flashLight.GetShadowMap()->GetShadowWidth());
        flashLight.GetShadowMap()->Write();
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderScene(flashShadowShader);

        /************************** render normal scene using depth map ***********************************/
        glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mainShader.UseShader();
        mainShader.setMat4("projection", projection);
        mainShader.setMat4("view", camera.getViewMatrix());
        mainShader.setvec3("viewPos", camera.getCameraPosition());
        mainShader.setvec2("viewPort", glm::vec2(mainWindow.getBufferWidth(), mainWindow.getBufferHeight()));
        mainShader.setInt("lightTexture", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, lighttexure.GetTextureID());
        if (!lPressed && mainWindow.getsKeys()[4]){
            flashIsOn = !flashIsOn;
        }

        lPressed = mainWindow.getsKeys()[4];
        
        if (flashIsOn) {
            flashLight.applyFlash(mainShader);
        }
        else {
            flashLight.flashLightOFF(mainShader);
        }

        mazeWallLight.Draw(mainShader);
        flashLight.GetShadowMap()->Read(GL_TEXTURE2);
        mainShader.setInt("flashShadowMap", 2);
        mainShader.setInt("ourTexture", 1);
        RenderScene(mainShader);
        //map.Draw(shaderList[0], projection, camera.getViewMatrix());

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
