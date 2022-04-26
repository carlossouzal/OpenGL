#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

#include "service/buffer/VertexBuffer.h"
#include "service/array/VertexArray.h"
#include "service/layout/VertexBufferLayout.h"

const int gWindowWidth = 800;
const int gWindowHeight = 600;
const char* TITLE = { "Hello Triangle - part I" };
GLFWwindow* gWindow = NULL;

bool initOpenGL();

static GLuint compileShader(const std::string& source, GLenum type) {
    GLuint id = glCreateShader(type);
    const GLchar* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        GLchar* message = (GLchar*)malloc(length * sizeof(GLchar));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compil e" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLint createProgram(const std::string& vertexSource,const std::string& fragmentSource) {
    GLuint program = glCreateProgram();
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

static std::string parseShader(const std::string& file) {
    std::ifstream stream(file);
    std::string line;
    std::stringstream ss;
    while (getline(stream, line)) {
        ss << line << '\n';
    }
    return ss.str();
}

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
    const std::string vertexShader = parseShader("shader/vertexShader.glsl");
    const std::string fragmentShader = parseShader("shader/fragmentShader.glsl");
    GLint shaderProgram = createProgram(vertexShader, fragmentShader);

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(gWindow))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Render the triangle
        glUseProgram(shaderProgram);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao.unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(gWindow);
    }

    //Clean UP
    glDeleteProgram(shaderProgram);
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