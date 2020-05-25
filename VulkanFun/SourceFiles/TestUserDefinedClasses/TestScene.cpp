#include "TestScene.h"
#include "TestComponent.h"
#include "TestComponent2.h"

void TestScene::PrepareScene()
{
    auto gameObject = GameObject::Create("dupa");
    gameObject.lock()->AddComponent<TestComponent2>();
}