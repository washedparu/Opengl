#pragma once
#include "debug.h"

#include "VertexArray.h"
#include "BufferLayout.h"
#include "VertexBuffer.h"

VertexArray::VertexArray() 
	: m_RendererID(0) 
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}


void VertexArray::UnBind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const BufferLayout& layout) {
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    uintptr_t offset = 0;

    for (unsigned int v = 0; v < elements.size(); v++) {
        const auto& elm = elements[v];
        GLCall(glEnableVertexAttribArray(v));
        GLCall(glVertexAttribPointer(v,
            elm.count,
            elm.type,
            elm.normalized,
            layout.GetStride(),
            (const void*)offset));

        offset += elm.count * vbe::getTypeSize(elm.type);
    }

}