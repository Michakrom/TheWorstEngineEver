#pragma once
#include <glm/glm.hpp>
#include "Component.h"

class Transform : public Component
{
public:
    Transform();
    Transform(glm::vec3 position, glm::vec3 rotation);

    glm::vec3 GetPosition() const;
    void SetPosition(glm::vec3 position);
    glm::vec3 GetRotation();
    void SetRotation(glm::vec3 rotation);

    void Start();
    void Update();

private:
    glm::vec3 position;
    glm::vec3 rotation;
};