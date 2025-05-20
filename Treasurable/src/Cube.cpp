#include "Cube.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"

void Cube::Draw(const glm::mat4& proj, const glm::mat4& view, Shader& shader, Renderer& renderer, VertexArray& vao, IndexBuffer& ibo) {

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Position);
	model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(0.5f, 1.0f, 0.0f));
	glm::mat4 mvp = proj * view * model;
	shader.Bind();
	shader.SetUniformMat4("u_MVP", mvp);
	renderer.DrawElements(vao, ibo, shader);
}