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
#include"WallLightWithShadows.h"


Window mainWindow;

Shader mainShader;
Shader flashShadowShader;
Shader omniShadowShader;
Shader debugDepthQuad;

Floor mazeFloor;
Wall mazeWall;
MazeMap mazeMap;
WallLight mazeWallLight;
FlashLight flashLight;
WallLightWithShadows wallLightWithShadows;
Material wallMaterial;
Material floorMaterial;

GLuint mazeWidth=10, mazeHeight=10;
GLfloat gridSize=0.8f;

GLuint WLShadowNumber = 10;
Camera camera;

GLfloat omniShadowFarPlane = 100.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


bool flashIsOn = false;
bool lPressed = false;

void RenderScene(const Shader &shader) {
    //render floor
    floorMaterial.UseMaterial(shader);
    mazeFloor.Draw();

    //render wall
    wallMaterial.UseMaterial(shader);
    mazeWall.Draw();

}

void RenderLightCube(const Shader& Lightingshader) {
    //mazeWallLight.Draw(Lightingshader.GetTransformLocation());
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void OmniShadowMapPass(glm::vec3 wallLight,OmniShadowMap omniShadowMap) {

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(wallLight, wallLight + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(wallLight, wallLight + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(wallLight, wallLight + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(wallLight, wallLight + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(wallLight, wallLight + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(wallLight, wallLight + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, omniShadowMap.GetShadowWidth(), omniShadowMap.GetShadowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    omniShadowMap.Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    omniShadowShader.UseShader();
    
    for (unsigned int i = 0; i < 6; ++i) {
        omniShadowShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    omniShadowShader.setFloat("far_plane", omniShadowFarPlane);
    omniShadowShader.setvec3("lightPos", wallLight);
    omniShadowShader.Validate();

    RenderScene(omniShadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main()
{
    
    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();

    
    mainShader.CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    flashShadowShader.CreateFromFiles("Shaders/FlashShadowShader.vert", "Shaders/FlashShadowShader.frag");
    debugDepthQuad.CreateFromFiles("Shaders/DebugShadowShader.vert", "Shaders/DebugShadowShader.frag");
    omniShadowShader.CreateFromFiles("Shaders/OmniShadowShader.vert", "Shaders/OmniShadowShader.gs", "Shaders/OmniShadowShader.frag");
    mazeMap = MazeMap(10, 10, 0.8f);
    mazeFloor = Floor(mazeWidth, mazeHeight, gridSize);
    mazeWall = Wall(mazeMap.GetWalls(),mazeMap.GetWallCount(), gridSize);
    mazeWallLight = WallLight(mazeMap.GetWallLights(), mazeMap.GetWallLightCount());
    wallMaterial = Material(0.3f, 3.0f);
    floorMaterial = Material(0.8f, 256.0f);
    camera = Camera(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, -1.0f),
        0.0f, 0.0f,0.0f, 5.0f, 0.5f);
    flashLight = FlashLight(camera.getCameraPosition(), camera.getCameraDirection());
    //flashLight = FlashLight(glm::vec3(0.0f,1.0f,1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    wallLightWithShadows = WallLightWithShadows(mazeMap.GetWallLights(), mazeMap.GetWallLightCount(), WLShadowNumber);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    //GLfloat i = 0;
    GLuint ShadowWallLightNumber = wallLightWithShadows.GetSetWLShadowNumber();
    std::vector<glm::vec3> choosenWallLights = wallLightWithShadows.GetWLShadow(camera.getCameraPosition());
    glm::vec3 lastPos = camera.getCameraPosition();

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
        //flashLight.SetFlashLight(glm::vec3(1.0f,1.0f,0.0f), glm::vec3(0.0f, -1.0f, -1.0f));
        glm::vec3 position = camera.getCameraPosition();
        position.y += 0.02f;
        position.x += 0.02f;
        glm::vec3 direction = camera.getCameraDirection();
        direction.y += 0.01f;
        direction.x += 0.01f;
        flashLight.SetFlashLight(position, direction);
        /************************ render depth scene from flashlight's perspective ***************************/
        
        flashShadowShader.UseShader();
        flashShadowShader.setMat4("lightSpaceMatrix", flashLight.GetLightSpaceMatrix());

        glViewport(0, 0, flashLight.GetShadowMap()->GetShadowHeight(), flashLight.GetShadowMap()->GetShadowWidth());
        flashLight.GetShadowMap()->Write();
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderScene(flashShadowShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        /************************** render depth scene from wallLights' perspective ***********************************/
        
        if (lastPos!=camera.getCameraPosition())
        {
            choosenWallLights = wallLightWithShadows.GetWLShadow(camera.getCameraPosition());
            lastPos = camera.getCameraPosition();
        }
        
        std::vector<OmniShadowMap> shaowmaps = wallLightWithShadows.GetShadowMap();
        
        for (size_t i = 0; i < ShadowWallLightNumber; i++) {
            OmniShadowMapPass(choosenWallLights[i], shaowmaps[i]);
        }
        



        /************************** render normal scene using depth map ***********************************/
        glViewport(0, 0, 1920, 1080);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mainShader.UseShader();
        mainShader.setMat4("projection", projection);
        mainShader.setMat4("view", camera.getViewMatrix());
        mainShader.setvec3("viewPos", camera.getCameraPosition());
        mainShader.setvec2("viewPort", glm::vec2(mainWindow.getBufferWidth(), mainWindow.getBufferHeight()));//mainWindow.getBufferWidth(), mainWindow.getBufferHeight()
        mainShader.setMat4("lightSpaceMatrix", flashLight.GetLightSpaceMatrix());

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

        for (size_t i = 0; i < ShadowWallLightNumber; i++) {
            shaowmaps[i].Read(GL_TEXTURE0 + 4 + i);
            mainShader.setInt("omniShadowMaps[" + std::to_string(i) + "].shadowMap", 4 + i);
            mainShader.setFloat("omniShadowMaps[" + std::to_string(i) + "].farPlane", omniShadowFarPlane);
            
        }
        
        
        RenderScene(mainShader);
        /*************************** render light cubes **********************************************/
        mazeWallLight.DrawLightCubes(projection, camera.getViewMatrix());


        /***************************** render light view for Testing ********************************/
        //glViewport(0, 0, 1920, 1080);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //debugDepthQuad.UseShader();
        //debugDepthQuad.setFloat("near_plane", 0.1f);
        //debugDepthQuad.setFloat("far_plane", 10.0f);
        //flashLight.GetShadowMap()->Read(GL_TEXTURE0);
        //renderQuad();

        //glUseProgram(0);


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
