#pragma once
// Includes
#define GLEW_STATIC
#include <GL/glew.h>


#include <iostream>
#include <memory>

// ASSERT macro 

#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__ ))

void inline GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool inline GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
