#include "Window.h"

Window::Window()
{
	width = 1920;
	height = 1080;
    offsetX = 0.0f;
    offsetY = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
    offsetX = 0.0f;
    offsetY = 0.0f;
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
    glfwSetCursorPosCallback(mainWindow, mouseCallback);

    //Set cursor inside the window
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    glfwSetWindowUserPointer(mainWindow, this);
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

GLfloat Window::getXchange()
{
    GLfloat theChange = offsetX;
    offsetX = 0.0f;
    return theChange;
}

GLfloat Window::getYchange()
{
    GLfloat theChange = offsetY;
    offsetY = 0.0f;
    
    return theChange;
}

//glm::quat Window::getQuatChange()
//{
//    return quatChange;
//}



GLfloat Window::getBufferWidth()
{
    return bufferWidth;
}

GLfloat Window::getBufferHeight()
{
    return bufferHeight;
}

void Window::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    xPos = static_cast<float>(xPos);
    yPos = static_cast<float>(yPos);
    if (theWindow->mouseFirstMove) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMove = false;
    }
    theWindow->offsetX = xPos - theWindow->lastX;
    theWindow->offsetY = yPos - theWindow->lastY;
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    float sensitivity = 0.0005f;
    theWindow->offsetX *= sensitivity;
    theWindow->offsetY *= sensitivity;
    


}
