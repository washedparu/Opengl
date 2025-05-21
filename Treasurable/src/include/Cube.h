#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Renderer.h"

class VertexArray;
class IndexBuffer;

class Object {
public:
    Object(glm::vec3 position, float angle)
        : m_Position(position), m_Angle(angle) {
        std::cout << "Object constructor\n";
    }
    virtual ~Object() { std::cout << "Object de-Constructor\n"; }

    virtual glm::vec3& GetPosition() { return m_Position; }
    virtual float& GetAngle() { return m_Angle; }


protected:
    glm::vec3 m_Position;
    float m_Angle;
};


class Cube : public Object {
public:
    Cube(glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f))
        : Object(position, 0.0f), m_Rotation(rotation) {
        std::cout << "Cube constructor\n";
    }
    ~Cube() override { std::cout << "Cube de-Constructorn\n"; }

    void Draw(const glm::mat4& proj, const glm::mat4& view,
        Shader& shader, Renderer& renderer,
        VertexArray& vao, IndexBuffer& ibo);


    glm::vec3& GetRotation() { return m_Rotation; }
private:
    glm::vec3 m_Rotation;
};
