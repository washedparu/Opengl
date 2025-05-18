#pragma once
#include "debug.h"

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer {
private:
	unsigned int m_RendererID;
public:
	Renderer();
	~Renderer();
	void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void Clear() const;
};