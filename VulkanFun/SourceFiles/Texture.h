#pragma once
#include <vulkan/vulkan.h>

typedef unsigned char stbi_uc;

struct Texture
{
    // TODO: Make it more secure
    stbi_uc *buffer;
    int width;
    int height;
    VkFormat format;

    ~Texture();
};