#pragma once
#include<GL/glew.h>
#include<string>
#include<unordered_map>
#include<glm/glm.hpp>

class Shader {
private:
	GLuint shader_id;
	std::string fileVertexShader;
	std::string fileFragmentShader;
	std::unordered_map<std::string, GLuint> locationCache;

public:
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);
	void setUniformMatrix4fv(const std::string& name, glm::mat4 matrix);

private:
	GLuint getUniformLocation(const std::string& name);
	GLuint compileShader(const std::string& source, GLenum type);
	GLint createProgram(const std::string& vertexSource, const std::string& fragmentSource);
	std::string parseShader(const std::string& file);
};