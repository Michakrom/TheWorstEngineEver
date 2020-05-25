#pragma once
#include <vector>
#include <list>
#include "Renderer.h"
#include "Vertex.h"
#include "GameObject.h"

class Scene
{
public:
    virtual ~Scene() = 0;

    void UpdateGameObjects();
    void RagisterGameObject(std::shared_ptr<GameObject> gameObject);
    void Destroy();


    virtual void PrepareScene() = 0;
private:
    std::list<std::shared_ptr<GameObject>> gameObjects;

    void RemoveGameObjectsMarkedForDestruction();
};