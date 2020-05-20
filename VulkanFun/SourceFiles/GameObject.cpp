#include "GameObject.h"
#include "Core.h"
#include "Transform.h"

#include <typeinfo>

const std::string GameObject::DEFAULT_NAME = "GameObject";

std::weak_ptr<GameObject> GameObject::Create()
{
    return Create(DEFAULT_NAME);
}
std::weak_ptr<GameObject> GameObject::Create(std::string name)
{
    auto gameObject = std::shared_ptr<GameObject>(new GameObject());
    gameObject->name = name;
    Core::GetCurrentScene()->RagisterGameObject(gameObject);

    gameObject->AddComponent<Transform>();

    return gameObject;
}

void GameObject::Destroy()
{
    for (std::shared_ptr<Component> component : components)
    {
        component->Destroy();
    }
    Core::GetCurrentScene()->RemoveGameObject(shared_from_this());
}

std::string GameObject::GetName()
{
    return name;
}

void GameObject::SetName(std::string name)
{
    this->name = name;
}

void GameObject::UpdateComponents()
{
    for (auto component : components)
    {
        component->Update();
    }
}


GameObject::GameObject() {}