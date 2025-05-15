#pragma once
#include "GL/glew.h"
#include <vector>
#include "debug.h"

struct vbe {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getTypeSize(unsigned int t) {
        switch (t) {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        ASSERT(false);
        return 0;
    }
};

class BufferLayout {
private:
    int m_Stride = 0;
    std::vector<vbe> m_Elements;
public:
    BufferLayout() { this->m_Stride = 0; }
    BufferLayout(const BufferLayout& copy) {
        printf("Copied BufferLayout!\n");
    }
    template<typename T>
    void push(unsigned int count);

    const std::vector<vbe>& GetElements() const { return m_Elements; }
    int GetStride() const { return m_Stride; }
};



template<>
inline void BufferLayout::push<float>(unsigned int count) {
    m_Elements.push_back({ GL_FLOAT, static_cast<unsigned int>(count), false });
    m_Stride += vbe::getTypeSize(GL_FLOAT) * count;
}

template<>
inline void BufferLayout::push<unsigned int>(unsigned int count) {
    m_Elements.push_back({ GL_UNSIGNED_INT, static_cast<unsigned int>(count), GL_FALSE });
    m_Stride += vbe::getTypeSize(GL_UNSIGNED_INT) * count;
}

template<>
inline void BufferLayout::push<unsigned char>(unsigned int count) {
    m_Elements.push_back({ GL_UNSIGNED_BYTE, static_cast<unsigned int>(count), GL_TRUE });
    m_Stride += vbe::getTypeSize(GL_UNSIGNED_BYTE) * count;
}