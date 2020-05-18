#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 rotation) : 
    position(position), rotation(rotation) {}

void Transform::Start() {}

void Transform::Update() {}

glm::vec3 Transform::GetPosition() const
{
    return position;
}
void Transform::SetPosition(glm::vec3 position)
{
    this->position = position;
}
glm::vec3 Transform::GetRotation()
{
    return rotation;
}
void Transform::SetRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
}