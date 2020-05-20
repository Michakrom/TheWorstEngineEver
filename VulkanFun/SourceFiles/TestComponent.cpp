#include "TestComponent.h"
#include "GameObject.h"
#include "Transform.h"

#include <glm/glm.hpp>


void TestComponent::Start()
{

}

void TestComponent::Update()
{
    gameObject.lock()->GetComponent<Transform>().lock()->SetPosition(
        glm::vec3(shift, 0, 0)
    );
    shift += 0.0001f;
}

void TestComponent::Destroy()
{

}