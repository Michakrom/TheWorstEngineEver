#pragma once

#include <memory>

class GameObject;

class Component
{
public:
    virtual ~Component() = 0;

    virtual void Start();
    virtual void Update();
    virtual void Destroy();

    virtual void SetGameObject(std::weak_ptr<GameObject> gameObject);
    virtual std::weak_ptr<GameObject> GetGameObject();

protected:
    std::weak_ptr<GameObject> gameObject;
};