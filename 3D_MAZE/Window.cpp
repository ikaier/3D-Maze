#include "Window.h"

Window::Window()
{
	width = 1920;
	height = 1080;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

void Window::Initialize()
{
    if (!glfwInit()) {
        printf("glfw initialisation failed");
        glfwTerminate();
        return;
    }

    //openGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create mian window
    mainWindow = glfwCreateWindow(width, height, "3D MAZE", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return;
    }

    //Get Buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //set context for glew to use (usefull if there are multiple windows)
    glfwMakeContextCurrent(mainWindow);

    //Allow moder extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("GLEW initilisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return;
    }
    glEnable(GL_DEPTH_TEST);
    //setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
}

bool Window::getShouldClose()
{
	return glfwWindowShouldClose(mainWindow);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(mainWindow);
}

void Window::processInput(GLfloat deltaTime)
{
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mainWindow, true);
    }

    keys[0] = 0.0f;
    keys[1] = 0.0f;
    keys[2] = 0.0f;
    keys[3] = 0.0f;

    if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS) {
        keys[0] = deltaTime;
        printf("w pressed\n");
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS) {
        keys[1] = deltaTime;
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS) {
        keys[2] = deltaTime;
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS) {
        keys[3] = deltaTime;
    }

}

GLfloat* Window::getsKeys()
{
    return keys;
}

GLfloat Window::getBufferWidth()
{
    return bufferWidth;
}

GLfloat Window::getBufferHeight()
{
    return bufferHeight;
}
