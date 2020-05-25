#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "Texture.h"

struct Character
{
    std::unique_ptr<Texture> texture;
    glm::vec2 bearing;
    int advance;
};