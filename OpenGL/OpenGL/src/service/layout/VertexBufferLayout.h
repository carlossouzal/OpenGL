#pragma once
#include<GL/glew.h>
#include<vector>
#include <type_traits>

struct VertexBufferElement
{
	GLuint type;
	GLuint count;
	GLubyte normalized;
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> elements;
	GLuint stride;

public:
	VertexBufferLayout()
		:stride(0){};

	template<typename T>
	void push(GLuint count) {}

	template<>
	void push<GLfloat>(GLuint count) {
		elements.push_back({GL_FLOAT, count, GL_FALSE});
		stride += count * sizeof(GL_FLOAT);
	}

	inline const std::vector<VertexBufferElement> getElements() const { return elements; }
	inline GLuint getStride() const { return stride; }
};