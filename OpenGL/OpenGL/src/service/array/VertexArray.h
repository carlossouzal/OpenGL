#pragma once

#include "../buffer/VertexBuffer.h"
#include "../layout/VertexBufferLayout.h"
class VertexArray {
private:
	GLuint id;

public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};