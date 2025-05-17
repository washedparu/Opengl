#include "include/Renderer.h"
#include "include/Shader.h"
#include <fstream>
#include <sstream>
#

Shader::Shader(const std::string& vertShaderPath, const std::string& fragShaderPath)
    : m_VertShaderPath(vertShaderPath), m_FragShaderPath(fragShaderPath), m_RendererID(0)
{
    ShaderPath src = ReadShaderFromFile(vertShaderPath, fragShaderPath);
    m_RendererID = CreateShader(src.VertShader, src.FragShader);

    GLCall(glUseProgram(m_RendererID));
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        std::vector<char> message(length);
        GLCall(glGetShaderInfoLog(id, length, &length, message.data()));
        std::cout << "Failed to compile {} shader " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << message.data();
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertShader, const std::string& fragShader) {
    unsigned int program = glCreateProgram(); 
    unsigned int vs = CompileShader(vertShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragShader, GL_FRAGMENT_SHADER);
    if (!vs || !fs) {
        GLCall(glDeleteProgram(program));
        return 0;
    }

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}


Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}


ShaderPath Shader::ReadShaderFromFile(const std::string& vertPath, const std::string& fragPath) {
    std::ifstream vertFile(vertPath);
    std::ifstream fragFile(fragPath);

    if (!vertFile.is_open()) {
        std::cout << "Failed to open vertex shader file: {}", vertPath;
        return {};
    }
    if (!fragFile.is_open()) {
        std::cout << "Failed to open fragment shader file: {}", fragPath;
        return {};
    }

    std::stringstream vertBuffer, fragBuffer;
    vertBuffer << vertFile.rdbuf();
    fragBuffer << fragFile.rdbuf();

    return ShaderPath{ vertBuffer.str(), fragBuffer.str() };
}



void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float f) {
    GLCall(glUniform1f(GetUniformLoc(name), f));
}


void Shader::SetUniform2f(const std::string& name, float f1, float f2) {
    GLCall(glUniform2f(GetUniformLoc(name), f1, f2));
}


void Shader::SetUniform3f(const std::string& name, float f1, float f2, float f3) {
    GLCall(glUniform3f(GetUniformLoc(name), f1, f2, f3));
}

void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4) {
    GLCall(glUniform4f(GetUniformLoc(name), f1, f2, f3, f4));
}



void Shader::SetUniform1i(const std::string& name, int i) {
    GLCall(glUniform1i(GetUniformLoc(name), i));
}


void Shader::SetUniform2i(const std::string& name, int i1, int i2) {
    GLCall(glUniform2i(GetUniformLoc(name), i1, i2));
}

void Shader::SetUniform3i(const std::string& name, int i1, int i2, int i3) {
    GLCall(glUniform3i(GetUniformLoc(name), i1, i2, i3));
}

void Shader::SetUniform4i(const std::string& name, int i1, int i2, int i3, int i4) {
    GLCall(glUniform4i(GetUniformLoc(name), i1, i2, i3, i4));
}



void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat) {
    GLCall(glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &mat[0][0] ));;
}



unsigned int Shader::GetRendererID() const { return m_RendererID; }

int Shader::GetUniformLoc(const std::string& name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));  
    if (location == -1) {
        std::cout << "Couldn't get uniform location for " << name.c_str() << '\n';
        return -1;
    }

    m_UniformLocationCache[name] = location;

    return location;
}