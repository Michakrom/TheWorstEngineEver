#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Component.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
    static std::weak_ptr<GameObject> Create();
    static std::weak_ptr<GameObject> Create(std::string name);

    void Destroy();

    std::string GetName();
    void SetName(std::string name);

    template <typename T>
    std::weak_ptr<T> AddComponent()
    {
        auto component = std::make_shared<T>();
        components.push_back(component);
        component->SetGameObject(shared_from_this());
        component->Start();
        return component;
    }
    template <typename T>
    std::weak_ptr<T> GetComponent();

private:
    static const std::string DEFAULT_NAME;

    std::string name;
    std::vector<std::shared_ptr<Component>> components;

    GameObject();
};