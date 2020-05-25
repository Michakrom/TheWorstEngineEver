#include "TestComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Core.h"
#include "InitialScene.h"
#include "TestScene.h"
#include "TextMaker.h"
#include "GameObject.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <stb_image.h>

void TestComponent::Start()
{
}

void TestComponent::Update()
{
    gameObject.lock()->GetComponent<Transform>().lock()->SetPosition(
        glm::vec3(shift, 2, 0));
    shift += 0.0003f;

        static int counter = 0;
    if (shift >= 0.5f)
    {
        // gameObject.lock()->Destroy();
        Core::SwitchScene<TestScene>();
        // auto texture = std::make_shared<Texture>();
        // TextMaker::Initialize();
        // FT_Bitmap bitmap = TextMaker::GetCharImage('U');
        // texture->buffer = bitmap.buffer;
        // texture->width = bitmap.width;
        // texture->height = bitmap.rows;
        // texture->format = VK_FORMAT_R8_UNORM;

        // auto gameObject = GameObject::Create("dupa");
        // gameObject.lock()->GetComponent<Transform>().lock()->SetPosition(
        //     glm::vec3(-2.0f + counter * 1, 0, 0));
        // auto rend = gameObject.lock()->AddComponent<Renderer>();
        // rend.lock()->SetVertices(std::make_shared<std::vector<Vertex>>(vertices3));
        // rend.lock()->SetIndices(std::make_shared<std::vector<uint32_t>>(indices3));
        // rend.lock()->SetTexture(texture);
        // rend.lock()->Initialize();
        // // shift = -2;
        // counter++;
    }
}

void TestComponent::Destroy()
{
}