#pragma once
#include <string>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>


struct ShaderPath {
    std::string VertShader;
    std::string FragShader;
};

class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_FragShaderPath;
    std::string m_VertShaderPath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    int GetUniformLoc(const std::string& name);

    unsigned int CreateShader(const std::string& vertShader, const std::string& fragShader);
    unsigned int CompileShader(const std::string& source, unsigned int type);
    ShaderPath ReadShaderFromFile(const std::string& vertPath, const std::string& fragPath);

public:
    Shader(const std::string& vertShaderPath, const std::string& fragShaderPath);
    ~Shader();


    void Bind() const;
    void UnBind() const;

    void SetUniform1f(const std::string& name, float f);
    void SetUniform2f(const std::string& name, float f1, float f2);
    void SetUniform3f(const std::string& name, float f1, float f2, float f3);
    void SetUniform4f(const std::string& name, const glm::vec4& v);

    void SetUniform1i(const std::string& name, int i);
    void SetUniform2i(const std::string& name, int i1, int i2);
    void SetUniform3i(const std::string& name, int i1, int i2, int i3);
    void SetUniform4i(const std::string& name, int i1, int i2, int i3, int i4);


    void SetUniformMat4(const std::string& name, const glm::mat4& mat);
    unsigned int GetRendererID() const;
};