#include "GameObject.h"
#include "Core.h"
#include <typeinfo>

const std::string GameObject::DEFAULT_NAME = "GameObject";

std::weak_ptr<GameObject> GameObject::Create()
{
    auto gameObject = std::shared_ptr<GameObject>(new GameObject());
    gameObject->name = DEFAULT_NAME;
    Core::GetCurrentScene()->RagisterGameObject(gameObject);
    return gameObject;
}
std::weak_ptr<GameObject> GameObject::Create(std::string name)
{
    auto gameObject = std::shared_ptr<GameObject>(new GameObject());
    gameObject->name = name;
    Core::GetCurrentScene()->RagisterGameObject(gameObject);
    std::weak_ptr test = gameObject;
    std::shared_ptr dupa = test.lock();
    return gameObject;
}

GameObject::GameObject() {}

void GameObject::Destroy()
{
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

template <typename T>
std::weak_ptr<T> GameObject::GetComponent()
{
    for (Component component : components)
    {
        if (typeinfo(T) == typeid(component))
            return component;
    }
}