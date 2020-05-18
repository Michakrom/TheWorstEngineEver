#include "Scene.h"
#include "Core.h"
#include "TextMaker.h"
#include "Texture.h"
#include <stb_image.h>
#include "ModelLoader.h"

void Scene::RagisterGameObject(
    std::shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(
    std::shared_ptr<GameObject> gameObject)
{
    gameObjects.remove(gameObject);
}

void Scene::PrepareScene()
{
    auto texture = std::make_shared<Texture>();
    TextMaker::Initialize();
    FT_Bitmap bitmap = TextMaker::GetCharImage('D');
    texture->buffer = bitmap.buffer;
    texture->width = bitmap.width;
    texture->height = bitmap.rows;
    texture->format = VK_FORMAT_R8_UNORM;

    auto gameObject = GameObject::Create("dupa");
    auto rend = gameObject.lock()->AddComponent<Renderer>();
    rend.lock()->SetVertices(std::make_shared<std::vector<Vertex>>(vertices3));
    rend.lock()->SetIndices(std::make_shared<std::vector<uint32_t>>(indices3));
    rend.lock()->SetTexture(texture);
    rend.lock()->shift = -1.6f;
    rend.lock()->Initialize();

    // gameObjects.reserve(2);
    // gameObjects.emplace_back();

    // gameObjects[0].SetVertices(std::make_shared<std::vector<Vertex>>(vertices3));
    // gameObjects[0].SetIndices(std::make_shared<std::vector<uint32_t>>(indices3));
    // gameObjects[0].SetTexture(texture);
    // gameObjects[0].shift = -1.6f;
    // gameObjects[0].Create();

    // auto texture2 = std::make_shared<Texture>();
    // texture2->buffer = stbi_load(
    //     TEXTURE_PATH.c_str(), &(texture2->width),
    //     &texture2->height, nullptr, STBI_rgb_alpha);
    // texture2->format = VK_FORMAT_R8G8B8A8_SRGB;

    // gameObjects.emplace_back();
    // std::vector<Vertex> vertices;
    // std::vector<uint32_t> indices;
    // ModelLoader::LoadModelAtPath(MODEL_PATH, &vertices, &indices);

    // gameObjects[1].SetVertices(std::make_shared<std::vector<Vertex>>(vertices));
    // gameObjects[1].SetIndices(std::make_shared<std::vector<uint32_t>>(indices));
    // gameObjects[1].SetTexture(texture2);
    // gameObjects[1].shift = 0.0f;
    // gameObjects[1].Create();

    // Core::GetVulkanHandler()->SetGameObjects(&gameObjects);
}
void Scene::UpdateGameObjects()
{
}