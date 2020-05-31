#include "InitialScene.h"
#include "Core.h"
#include "TextMaker.h"
#include "Texture.h"
#include "ModelLoader.h"
#include "TextRenderer.h"

#include <stb_image.h>
#include <memory>

void InitialScene::PrepareScene()
{
    auto textVector = TextMaker::CreateText("This is an\nexample scene.", 
        glm::vec3(-1, 0.1f, 0), 150, glm::vec3(0.0f, 0.6f, 0.05f));
}