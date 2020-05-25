#pragma once
#include <vulkan/vulkan.h>
#include <memory>

struct Texture
{
    std::unique_ptr<unsigned char> buffer;
    int width;
    int height;
    VkFormat format;
};