#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

Renderer::Renderer() { this->m_RendererID = 0; }
Renderer::~Renderer() {}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}