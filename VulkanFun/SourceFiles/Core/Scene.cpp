#include "Scene.h"
#include "Core.h"

#include <stb_image.h>

Scene::~Scene(){};

void Scene::Destroy()
{
    for (std::shared_ptr<GameObject> gameObject : gameObjects)
    {
        gameObject->Destroy();
    }
}

void Scene::RagisterGameObject(
    std::shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObjectsMarkedForDestruction()
{
    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
        if ((*it)->IsMarkedAsDestroyed())
        {
            gameObjects.erase(it--);
        }
    }
}

void Scene::UpdateGameObjects()
{
    for (auto gameObject : gameObjects)
    {
        if (!gameObject->IsJustCreated())
        {
            if (gameObject->IsActive() && !gameObject->IsMarkedAsDestroyed())
                gameObject->UpdateComponents();
        }
        else
        {
            gameObject->SetJustCreated(false);
        }
    }
    RemoveGameObjectsMarkedForDestruction();
}