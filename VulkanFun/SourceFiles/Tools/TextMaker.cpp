#include "TextMaker.h"
#include "iostream"
#include "Vertex.h"
#include "Renderer.h"

#include <vector>
#include <memory>

FT_Library TextMaker::library;
FT_Face TextMaker::face;

std::vector<Vertex> TextMaker::quadVertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};
std::vector<uint32_t> TextMaker::quadIndices = {
    0, 1, 2, 2, 3, 0};

void TextMaker::Initialize()
{
    bool error = FT_Init_FreeType(&library);
    if (error)
    {
        throw std::runtime_error("FreeType: Unable to initialize library");
    }

    error = FT_New_Face(library,
                        "C:/Users/Admin/workspace/C++/VulkanFun/VulkanFun/"
                        "Fonts/arial.ttf",
                        0,
                        &face);
    if (error == FT_Err_Unknown_File_Format)
    {
        throw std::runtime_error("FreeType: Unsupported font format");
    }
    else if (error)
    {
        throw std::runtime_error("FreeType: Unknown font loading error");
    }
}

void TextMaker::Deinitialize()
{
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}


std::shared_ptr<Character> TextMaker::CreateCharacterTexture(char character,
    int size)
{
    return CreateCharacterTexture(character, size, true);
}

std::shared_ptr<Character> TextMaker::CreateCharacterTexture(char character,
    int size, bool initialize)
{
    if (initialize)
        Initialize();

    FT_GlyphSlot slot = face->glyph;

    bool error = FT_Set_Pixel_Sizes(face, size, 0);

    error = FT_Load_Char(face, character, FT_LOAD_RENDER);

    if (error)
        throw std::runtime_error("FreeType: Unable to load character");

    auto texture = std::make_unique<Texture>();
    texture->width = slot->bitmap.width;
    texture->height = slot->bitmap.rows;

    // I need to copy the buffer, because it gets deleted in Deinitialize()
    auto temp = new unsigned char[texture->width * texture->height];
    std::copy(slot->bitmap.buffer, slot->bitmap.buffer +
        texture->width * texture->height, temp);
    texture->buffer = std::unique_ptr<unsigned char>(temp);

    texture->format = VK_FORMAT_R8_UNORM;

    auto result = std::make_shared<Character>();
    result->texture = std::move(texture);
    result->advance = slot->advance.x;
    result->bearing.x = slot->bitmap_left;
    result->bearing.y = slot->bitmap_top;

    if (initialize)
        Deinitialize();

    return result;
}

std::vector<std::weak_ptr<GameObject>> TextMaker::CreateText(std::string text,
    glm::vec3 position, int size)
{
    std::vector<std::weak_ptr<GameObject>> result;
    result.reserve(text.size());

    Initialize();

    float scale = size / 100000.0f;
    float spaceWidth = scale * 80;

    for (auto i = 0; i < text.size(); i++)
    {
        if (text[i] == ' ')
        {
            position.x += scale * 100;
            continue;
        }
    

        auto gameObject = GameObject::Create();
        auto rend = gameObject.lock()->AddComponent<Renderer>();

        std::shared_ptr<Character> character = 
            CreateCharacterTexture(text[i], size, false);

        float xpos = position.x + character->bearing.x * scale;
        float ypos = position.y - (character->texture->height - 
            character->bearing.y) * scale;

        float width = character->texture->width * scale;
        float height = character->texture->height * scale;

        std::vector<Vertex> vertices = {
            {{xpos, ypos, 0.0f},
                {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
            {{xpos + width, ypos, 0.0f},
                {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{xpos + width, ypos + height, 0.0f},
                {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{xpos, ypos + height, 0.0f},
                {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
        };


        rend.lock()->SetVertices(
            std::make_shared<std::vector<Vertex>>(vertices)
        );
        rend.lock()->SetIndices(
            std::make_shared<std::vector<uint32_t>>(quadIndices)
        );
        rend.lock()->SetTexture(std::move(character->texture));
        rend.lock()->Initialize();

        // auto tr = gameObject.lock()->GetComponent<Transform>();
        // tr.lock()->SetPosition(
        //     glm::vec3(position.x + i, position.y, position.z)
        // );
        
        position.x += (character->advance >> 6) * scale;
            
        result.push_back(gameObject);
    }

    Deinitialize();
    return result;
}