#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int gWindowWidth = 800;
const int gWindowHeight = 600;
const char* TITLE = { "Hello Window - part II" };
GLFWwindow* pWindow = NULL;

bool initOpenGL();

int main(void){

    if (!initOpenGL()){
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(pWindow)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();
    return 0;
}

bool initOpenGL()
{
    if (!glfwInit()){
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, TITLE, NULL, NULL);
    if (pWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(pWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    return true;
}