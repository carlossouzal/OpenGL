#pragma once
#include<GL/glew.h>

class IndexBuffer {
private:
	GLuint id;
	GLuint count;
public:
	IndexBuffer(const GLuint *data, GLuint count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline GLuint getCount() const { return count; }
};