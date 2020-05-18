#pragma once

#include <memory>

class GameObject;

class Component
{
public:
    virtual ~Component() = 0;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;

    virtual void SetGameObject(std::weak_ptr<GameObject> gameObject);
    virtual std::weak_ptr<GameObject> GetGameObject();

protected:
    std::weak_ptr<GameObject> gameObject;
};