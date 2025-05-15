#pragma once

#include <iostream>
class BufferLayout;
class VertexBuffer;

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray(const VertexArray& c) = delete;
    VertexArray();
    ~VertexArray();
    void addBuffer(const VertexBuffer& vb, const BufferLayout& layout);

    void Bind() const;
    void UnBind() const;
};