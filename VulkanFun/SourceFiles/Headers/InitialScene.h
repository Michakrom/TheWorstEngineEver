#pragma once
#include "Scene.h"

class InitialScene : public Scene
{
public:
    virtual void PrepareScene();
private:
    const std::string MODEL_PATH = "models/chalet.obj";
    const std::string TEXTURE_PATH = "textures/chalet.jpg";
    std::vector<Vertex> vertices3 = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};
    std::vector<uint32_t> indices3 = {
        0, 1, 2, 2, 3, 0};
};