#include "InitialScene.h"
#include "Core.h"
#include "TextMaker.h"
#include "Texture.h"
#include "TestComponent.h"
#include "ModelLoader.h"

#include <stb_image.h>
#include <memory>

void InitialScene::PrepareScene()
{
    // std::shared_ptr<Texture> texture = TextMaker::CreateCharacterTexture('D',
    //     256);

    auto textVector = TextMaker::CreateText("chuj kurwa", glm::vec3(-2, 0, 0), 256);

    auto textVector2 = TextMaker::CreateText("dupa cipa", glm::vec3(-2, -1, 0), 256);
    // auto gameObject = GameObject::Create("dupa");
    // auto rend = gameObject.lock()->AddComponent<Renderer>();
    // rend.lock()->SetVertices(std::make_shared<std::vector<Vertex>>(vertices3));
    // rend.lock()->SetIndices(std::make_shared<std::vector<uint32_t>>(indices3));
    // rend.lock()->SetTexture(texture);
    // rend.lock()->Initialize();

    // int* test = new int[5];
    // test[0] = 1;
    // test[1] = 2;
    // test[2] = 3;
    // test[3] = 4;
    // test[4] = 5;

    // std::shared_ptr<int> arr(test, [](int *p) { delete [] p; });
    // std::cout << arr.get()[2];
    // std::shared_ptr<int> arr(new int[5](), [](int *p) { delete [] p; });
    // auto test2 = std::make_shared<int>(new int[5], [](int *t) { delete[] t; });
    // auto test2 = std::make_shared<int>(test, [](int *t) { delete[] t; });
    // auto test3 = std::unique_ptr<int>(test);
    // auto test3 = std::shared_ptr<int>(test);
    // std::cout << test3.get()[2];

    // auto gameObject2 = GameObject::Create();
    // auto texture2 = std::make_shared<Texture>();
    // texture2->buffer = std::make_unique<unsigned char[]>(*stbi_load(
    //     TEXTURE_PATH.c_str(), &(texture2->width), &texture2->height, nullptr,
    //     STBI_rgb_alpha));
    // texture2->format = VK_FORMAT_R8G8B8A8_SRGB;

    // auto vertices = std::make_shared<std::vector<Vertex>>();
    // auto indices = std::make_shared<std::vector<uint32_t>>();
    // ModelLoader::LoadModelAtPath(MODEL_PATH, vertices.get(), indices.get());

    // auto rend2 = gameObject2.lock()->AddComponent<Renderer>();
    // rend2.lock()->SetVertices(vertices);
    // rend2.lock()->SetIndices(indices);
    // rend2.lock()->SetTexture(texture2);
    // rend2.lock()->Initialize();
    // gameObject.lock()->AddComponent<TestComponent>();
}