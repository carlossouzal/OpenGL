#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Shader::Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	: fileFragmentShader(fragmentFilepath), fileVertexShader(vertexFilepath)
{
    const std::string vertexShader = parseShader(fileVertexShader);
    const std::string fragmentShader = parseShader(fileFragmentShader);

    shader_id = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(shader_id);
}

GLuint Shader::compileShader(const std::string& source, GLenum type) {
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

void Shader::setUniform4f(const std::string& name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) {
    glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setUniformMatrix4fv(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint Shader::getUniformLocation(const std::string& name){
    if (locationCache.find(name) != locationCache.end())
        return locationCache[name];

    GLint location = glGetUniformLocation(shader_id, name.c_str());

    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist! " << std::endl;

    locationCache[name] = location;
    return location;
}

void Shader::bind() const {
    glUseProgram(shader_id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

std::string Shader::parseShader(const std::string& file) {
    std::ifstream stream(file);
    std::string line;
    std::stringstream ss;
    while (getline(stream, line)) {
        ss << line << '\n';
    }
    return ss.str();
}

GLint Shader::createProgram(const std::string& vertexSource, const std::string& fragmentSource) {
    GLint program = glCreateProgram();
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