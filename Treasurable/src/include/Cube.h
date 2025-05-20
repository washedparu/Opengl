#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Shader;
class Renderer;
class VertexArray;
class IndexBuffer;

class Object {
public:

	Object(glm::vec3 position, float angle)  : m_Position(position), m_Angle(angle) {}
	virtual ~Object() {}

	virtual glm::vec3& GetPosition() { return m_Position; }
	virtual float& GetAngle() { return m_Angle; }

protected:
	glm::vec3 m_Position;
	float m_Angle;
};

class Cube : public Object {
public:
	using Object::Object;
	void Draw(const glm::mat4& proj, const glm::mat4& view, Shader& shader, Renderer& renderer, VertexArray& vao, IndexBuffer& ibo);
};