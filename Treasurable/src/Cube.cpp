#include "Cube.h"
#include "VertexArray.h"


void Cube::Draw(const glm::mat4& proj, const glm::mat4& view,
    Shader& shader, Renderer& renderer,
    VertexArray& vao, IndexBuffer& ibo)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Position);
    model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0)); // Pitch
    model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0)); // Yaw
    model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1)); // Roll

    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4("u_MVP", mvp);

    renderer.DrawElements(vao, ibo, shader);
}