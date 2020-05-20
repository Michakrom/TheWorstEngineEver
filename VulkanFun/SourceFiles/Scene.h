#pragma once
#include <vector>
#include <list>
#include "Renderer.h"
#include "Vertex.h"
#include "GameObject.h"

class Scene
{
public:
    // Scene();
    // ~Scene();
    void PrepareScene();
    void UpdateGameObjects();
    void ObserveGameObject();
    void RagisterGameObject(std::shared_ptr<GameObject> gameObject);
    void RemoveGameObject(std::shared_ptr<GameObject> gameObject);
    void Destroy();

private:
    const std::string MODEL_PATH = "models/chalet.obj";
    const std::string TEXTURE_PATH = "textures/chalet.jpg";
    std::list<std::shared_ptr<GameObject>> gameObjects;
    std::vector<Vertex> vertices3 = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};
    std::vector<uint32_t> indices3 = {
        0, 1, 2, 2, 3, 0};
};