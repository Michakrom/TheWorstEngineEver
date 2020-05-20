#pragma once
#include <glm/glm.hpp>
#include "Component.h"

class Transform : public Component
{
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Destroy() override;

    glm::vec3 GetPosition() const;
    void SetPosition(glm::vec3 position);
    glm::vec3 GetRotation();
    void SetRotation(glm::vec3 rotation);

private:
    glm::vec3 position;
    glm::vec3 rotation;
};