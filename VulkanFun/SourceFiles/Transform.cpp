#include "Transform.h"

void Transform::Start()
{
}

void Transform::Update()
{

}
void Transform::Destroy()
{

}

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