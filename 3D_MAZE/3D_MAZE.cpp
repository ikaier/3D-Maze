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
#include"HDRProcess.h"
#include"Polyhedron.h"


Window mainWindow;

Shader mainShader;
Shader flashShadowShader;
Shader omniShadowShader;
Shader debugDepthQuad;
Shader HDRShader;
Shader BlurShader;


HDRProcess HDRframebuffer;

Polyhedron polys;
Floor mazeFloor;
Floor mazeCeiling;
Wall mazeWall;
MazeMap mazeMap;
WallLight mazeWallLight;
FlashLight flashLight;
Material wallMaterial;
Material floorMaterial;

GLuint mazeWidth=10, mazeHeight=10;
GLfloat gridSize=0.8f;

GLuint workingLightsNumber = 10;
Camera camera;

GLfloat omniShadowNearPlane = 0.01f;
GLfloat omniShadowFarPlane = 20.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat IconRotation = 0.0f;


bool flashIsOn = false;
bool lPressed = false;
bool gPressed = false;



void RenderScene(const Shader &shader) {
    //render floor
    floorMaterial.UseMaterial(shader);
    mazeFloor.Draw();

    floorMaterial.UseMaterial(shader);
    mazeCeiling.Draw();

    //render wall
    wallMaterial.UseMaterial(shader);
    mazeWall.Draw();

    floorMaterial.UseMaterial(shader);
    polys.Draw();
}


unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

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
    glDrawArrays(GL_TRIANGLES, 0,6);
    glBindVertexArray(0);
}

void OmniShadowMapPass(glm::vec3 wallLight,OmniShadowMap omniShadowMap) {

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, omniShadowNearPlane, omniShadowFarPlane);
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
    HDRframebuffer.InitBloom(1920, 1080);
    
    mainShader.CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    flashShadowShader.CreateFromFiles("Shaders/FlashShadowShader.vert", "Shaders/FlashShadowShader.frag");
    debugDepthQuad.CreateFromFiles("Shaders/DebugShadowShader.vert", "Shaders/DebugShadowShader.frag");
    omniShadowShader.CreateFromFiles("Shaders/OmniShadowShader.vert", "Shaders/OmniShadowShader.gs", "Shaders/OmniShadowShader.frag");
    HDRShader.CreateFromFiles("Shaders/HDRShader.vert", "Shaders/HDRShader.frag");
    BlurShader.CreateFromFiles("Shaders/BlurShader.vert", "Shaders/BlurShader.frag");
    

    mazeMap = MazeMap(mazeWidth, mazeHeight, 0.8f);
    mazeFloor = Floor(mazeWidth, mazeHeight, gridSize, "Textures/wood.png",0);
    mazeCeiling= Floor(mazeWidth, mazeHeight, gridSize, "Textures/ceiling.bmp", gridSize * 3 / 5);
    mazeWall = Wall(mazeMap.GetWalls(),mazeMap.GetWallCount(), gridSize);
    mazeWallLight = WallLight(mazeMap.GetWallLights(), mazeMap.GetWallLightCount(), workingLightsNumber);
    wallMaterial = Material(0.3f, 3.0f);
    floorMaterial = Material(0.8f, 256.0f);
    camera = Camera(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, -1.0f, -1.0f),
        0.0f, 0.0f,0.0f, 5.0f, 0.5f, mazeWidth, mazeHeight, gridSize);
    flashLight = FlashLight(camera.getCameraPosition(), camera.getCameraDirection());
    polys = Polyhedron(mazeMap.GetPolys(), mazeMap.GetPolyCount(), gridSize);

    //flashLight = FlashLight(glm::vec3(0.0f,1.0f,1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    GLuint ShadowWallLightNumber = mazeWallLight.GetSetWLShadowNumber();
    std::vector<glm::vec3> choosenWallLights = mazeWallLight.GetWLShadow(camera.getCameraPosition());
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
        if (!gPressed && mainWindow.getsKeys()[5]) {
            camera.GodModeToggle();
        }
        gPressed = mainWindow.getsKeys()[5];
        if (!camera.GodModeStatus())
        {
            glm::vec2 adjust=mazeMap.CollionDetection(camera.getCameraPosition());
            camera.CollionRes(adjust);
        }
        
        glm::vec3 position = camera.getCameraPosition();
        //printf("adjust is %f,%f,%f,\n", adjust.x, adjust.y, adjust.z);
        //printf("%f,%f\n", fmod((fmod(position.x, gridSize) + gridSize), gridSize), fmod((fmod(-position.z, gridSize) + gridSize), gridSize));
        



        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        position.y += 0.02f;
        position.x += 0.02f;
        glm::vec3 direction = camera.getCameraDirection();
        direction.y += 0.01f;
        direction.x += 0.01f;
        flashLight.SetFlashLight(position, direction);

        //polys rotation
        IconRotation += 45.0f* deltaTime;
        if (IconRotation >= 360.0f)IconRotation -= 360.0f;
        
        polys.Rotate(IconRotation);
        polys.Set();
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
            choosenWallLights = mazeWallLight.GetWLShadow(camera.getCameraPosition());
            lastPos = camera.getCameraPosition();
        }
        
        std::vector<OmniShadowMap> shaowmaps = mazeWallLight.GetShadowMap();
        
        for (size_t i = 0; i < ShadowWallLightNumber; i++) {
            OmniShadowMapPass(choosenWallLights[i], shaowmaps[i]);
        }
        



        /************************** render normal scene using depth map ***********************************/
        glViewport(0, 0, 1920, 1080);
        
        HDRframebuffer.WriteFloatBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.UseShader();
        mainShader.setMat4("projection", projection);
        mainShader.setMat4("view", camera.getViewMatrix());
        mainShader.setvec3("viewPos", camera.getCameraPosition());
        mainShader.setvec2("viewPort", glm::vec2(mainWindow.getBufferWidth(), mainWindow.getBufferHeight()));//mainWindow.getBufferWidth(), mainWindow.getBufferHeight()
        mainShader.setMat4("lightSpaceMatrix", flashLight.GetLightSpaceMatrix());
        mainShader.setInt("ShadowPointLightCount", ShadowWallLightNumber);
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
                /*************************** render light cubes **************************************/
        mazeWallLight.DrawLightCubes(projection, camera.getViewMatrix());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /***************************** Post Rendering Processing ********************************/
        bool horizontal = true, firstIter = true;
        GLuint blurTimes = 8;
        BlurShader.UseShader();
        //blur result of last blur with different orientation 10 times.
        for (size_t i = 0; i < blurTimes; i++) {
            HDRframebuffer.WriteColorBuffer(horizontal);
            BlurShader.setInt("horizontal", horizontal);
            if (firstIter) {
                HDRframebuffer.ReadFloatBuffer(GL_TEXTURE0, 1);
            }
            else {
                HDRframebuffer.ReadColorBuffer(GL_TEXTURE0, !horizontal);
            }
            renderQuad();
            horizontal = !horizontal;
            if (firstIter) {
                firstIter = false;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //combine result and blured result
        glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        HDRShader.UseShader();
        HDRShader.setFloat("exposure", 0.5f);
        HDRShader.setInt("scene", 0);
        HDRShader.setInt("bloomBlur", 1);
        HDRframebuffer.ReadFloatBuffer(GL_TEXTURE0,0);
        HDRframebuffer.ReadColorBuffer(GL_TEXTURE1, !horizontal);
        
        renderQuad();

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
