#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

#include "service/buffer/VertexBuffer.h"
#include "service/array/VertexArray.h"
#include "service/layout/VertexBufferLayout.h"
#include "service/shader/Shader.h"

const int gWindowWidth = 800;
const int gWindowHeight = 600;
const char* TITLE = { "Hello Triangle - part I" };
GLFWwindow* gWindow = NULL;

bool initOpenGL();

int main(void)
{

    if (!initOpenGL())
    {
        std::cerr << "Initialization failed" << std::endl;
        return -1;
    }

    //Array of vertices for a triangle
    GLfloat vertices[]{
        0.0f,  0.5f, 0.0f, //Top
        0.5f, -0.5f, 0.0f, //Right
       -0.5f, -0.5f, 0.0f, //Left
    };

    //Buffers on the GPU
    VertexBuffer vbo(vertices, sizeof(vertices));
    VertexArray vao;

    //Layouts
    VertexBufferLayout layout;
    layout.push<GLfloat>(3);
    vao.addBuffer(vbo, layout);

    //Create a program shader
    Shader program("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
    program.unbind();

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(gWindow))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Render the triangle
        program.bind();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao.unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(gWindow);
    }

    //Clean UP
    vao.~VertexArray();
    vbo.~VertexBuffer();

    glfwTerminate();
    return 0;
}

bool initOpenGL()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //only to support MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, TITLE, NULL, NULL);
    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(gWindow);


    //Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}