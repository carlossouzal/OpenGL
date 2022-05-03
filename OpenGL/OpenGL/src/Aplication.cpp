#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "service/buffer/VertexBuffer.h"
#include "service/array/VertexArray.h"
#include "service/layout/VertexBufferLayout.h"
#include "service/shader/Shader.h"
#include "service/index/IndexBuffer.h"

bool initOpenGL(GLFWwindow** gWindow);

int main(void)
{
    GLFWwindow* gWindow = NULL;

    if (!initOpenGL(&gWindow))
    {
        std::cerr << "Initialization failed" << std::endl;
        return -1;
    }

    glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);

    //Array of vertices for a triangle
    GLfloat vertices[]{
        -1.0f, 1.0f, 0.0f, //Top left
         1.0f, 1.0f, 0.0f, //Top Right
         1.0f, -1.0f, 0.0f, //Bottom Right
        -1.0f, -1.0f, 0.0f, //Bottom Left
    };

    GLuint index[]{
        0, 1, 2, //First Triangle
        0, 2, 3, //Second Triangle
    };

    //Buffers on the GPU
    VertexBuffer vbo(vertices, sizeof(vertices));
    VertexArray vao;
    //Layouts
    VertexBufferLayout layout;
    layout.push<GLfloat>(3);
    vao.addBuffer(vbo, layout);

    IndexBuffer ibo(index, 6);

    //Create a program shader
    Shader program("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
    program.bind();
    program.setUniformMatrix4fv("projection", projection);
    program.unbind();

    vbo.unbind();
    vao.unbind();
    ibo.unbind();

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
        ibo.bind();
        vao.bind();
        glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
        vao.unbind();
        ibo.unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(gWindow);
    }

    //Clean UP
    vao.~VertexArray();
    vbo.~VertexBuffer();
    ibo.~IndexBuffer();

    glfwTerminate();
    return 0;
}

bool initOpenGL(GLFWwindow** gWindow)
{
    const int gWindowWidth = 800;
    const int gWindowHeight = 600;
    const char* TITLE = { "Hello Triangle - part I" };

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    *gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, TITLE, NULL, NULL);
    if (*gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(*gWindow);


    //Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}