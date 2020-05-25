#include "TestComponent2.h"
#include "GameObject.h"
#include "Transform.h"
#include "Core.h"
#include "InitialScene.h"
#include "TestScene.h"

#include <glm/glm.hpp>

void TestComponent2::Update()
{
    gameObject.lock()->GetComponent<Transform>().lock()->SetPosition(
        glm::vec3(shift, 0, 0)
    );
    shift += 0.0001f;

    if (shift >= 0.9f)
        // gameObject.lock()->Destroy();
        Core::SwitchScene<InitialScene>();
}