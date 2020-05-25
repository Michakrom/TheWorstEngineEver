#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <memory>

#include "Texture.h"
#include "GameObject.h"
#include "glm/glm.hpp"
#include "Vertex.h"
#include "Character.h"

class TextMaker
{
public:
    static std::shared_ptr<Character> CreateCharacterTexture(char character,
        int size);
    static std::vector<std::weak_ptr<GameObject>>
        CreateText(std::string text, glm::vec3 position, int size);

private:
    static void Initialize();
    static void Deinitialize();
    static std::shared_ptr<Character> CreateCharacterTexture(char character,
        int size, bool initialize);
    static FT_Library library;
    static FT_Face face;

    static std::vector<Vertex> quadVertices;
    static std::vector<uint32_t> quadIndices;
};
