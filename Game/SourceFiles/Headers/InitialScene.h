#pragma once
#include "Scene.h"

class InitialScene : public Scene
{
public:
    void PrepareScene() override;

private:
    const std::string MODEL_PATH = "models/chalet.obj";
    const std::string TEXTURE_PATH = "textures/chalet.jpg";
};