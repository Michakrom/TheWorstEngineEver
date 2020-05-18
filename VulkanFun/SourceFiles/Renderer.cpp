#include "Renderer.h"
#include <glm/glm.hpp>
#include "TextMaker.h"
#include <stb_image.h>
#include "Core.h"

void Renderer::Start()
{
    // vulkanHandler->RagisterRenderer(shared_from_this());
}

void Renderer::Update()
{

}

void Renderer::Destroy()
{
    vulkanHandler->RemoveRenderer(shared_from_this());

    for (size_t i = 0; i < vulkanHandler->GetSwapChainImages().size();
         i++)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyImageView(device, textureImageView, nullptr);

    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
}

Renderer::Renderer()
{
    vulkanHandler = Core::GetVulkanHandler();
    device = vulkanHandler->GetDevice();
}

Renderer::Renderer(const std::shared_ptr<std::vector<Vertex>> vertices,
                   std::unique_ptr<std::vector<uint32_t>> indices,
                   const std::shared_ptr<Texture> texture)
    : vulkanHandler(vulkanHandler), vertices(vertices),
      indices(std::move(indices)), texture(texture)
{
    device = vulkanHandler->GetDevice();
}

Renderer::Renderer(const Renderer &rend)
{
    std::cout << "trying to copy renderer, it shouldn't happen";
}

Renderer::~Renderer() { }

void Renderer::Initialize()
{
    CreateTextureImage();
    CreateTextureImageView();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffer();
    CreateDescriptorSets();
    vulkanHandler->RagisterRenderer(shared_from_this());
}

std::vector<Vertex> *Renderer::GetVertices() const
{
    return vertices.get();
}
void Renderer::SetVertices(std::shared_ptr<std::vector<Vertex>> vertices)
{
    this->vertices = vertices;
}

std::vector<uint32_t> *Renderer::GetIndices() const
{
    return indices.get();
}
void Renderer::SetIndices(std::shared_ptr<std::vector<uint32_t>> indices)
{
    this->indices = indices;
}

std::shared_ptr<Texture> Renderer::GetTexture() const
{
    return texture;
}
void Renderer::SetTexture(std::shared_ptr<Texture> texture)
{
    this->texture = texture;
}

std::vector<VkDescriptorSet> *Renderer::GetDescriptorSets() const
{
    return descriptorSets.get();
}
void Renderer::SetDescriptorSets(
    std::unique_ptr<std::vector<VkDescriptorSet>> descriptorSets)
{
    this->descriptorSets = std::move(descriptorSets);
}

VkBuffer Renderer::GetVertexBuffer() const
{
    return vertexBuffer;
}
VkBuffer Renderer::GetIndexBuffer() const
{
    return indexBuffer;
}

void Renderer::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof((*vertices)[0]) *
                              vertices->size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    vulkanHandler->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory,
                0, bufferSize, 0, &data);
    memcpy(data, vertices->data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    vulkanHandler->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    vulkanHandler->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof((*indices)[0]) * indices->size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    vulkanHandler->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0,
                bufferSize, 0, &data);
    memcpy(data, indices->data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    vulkanHandler->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    vulkanHandler->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::CreateUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    size_t swapChainImagesSize =
        vulkanHandler->GetSwapChainImages().size();

    uniformBuffers.resize(swapChainImagesSize);
    uniformBuffersMemory.resize(swapChainImagesSize);

    for (size_t i = 0; i < swapChainImagesSize; i++)
    {
        vulkanHandler->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }

    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(shift, 0, 3.0f), glm::vec3(shift, 0.0f, 0.0f),
                           glm::vec3(0.0f, -1.0f, 1.0f));
    ubo.proj = glm::perspective(
        glm::radians(80.0f),
        vulkanHandler->GetSwapChainExtent().width /
            (float)vulkanHandler->GetSwapChainExtent().height,
        0.1f,
        10.0f);
    ubo.proj[1][1] *= -1;

    //ToDo: fix it
    void *data;
    vkMapMemory(device, uniformBuffersMemory[0], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[0]);

    vkMapMemory(device, uniformBuffersMemory[1], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[1]);

    vkMapMemory(device, uniformBuffersMemory[2], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[2]);
}

void Renderer::CreateTextureImageView()
{
    textureImageView = vulkanHandler->createImageView(
        textureImage, texture->format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Renderer::CreateTextureImage()
{
    stbi_uc *pixels = texture->buffer;

    VkDeviceSize imageSize;
    if (texture->format == VK_FORMAT_R8G8B8A8_SRGB)
        imageSize = texture->width * texture->height * 4;
    else
        imageSize = texture->width * texture->height;

    if (!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    vulkanHandler->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    vulkanHandler->createImage(texture->width, texture->height, texture->format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    vulkanHandler->transitionImageLayout(textureImage, texture->format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vulkanHandler->copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texture->width), static_cast<uint32_t>(texture->height));
    vulkanHandler->transitionImageLayout(textureImage, texture->format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(vulkanHandler->GetSwapChainImages().size(), vulkanHandler->GetDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = vulkanHandler->GetDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(vulkanHandler->GetSwapChainImages().size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets = std::make_unique<std::vector<VkDescriptorSet>>();
    descriptorSets->resize(vulkanHandler->GetSwapChainImages().size());
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets->data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < vulkanHandler->GetSwapChainImages().size(); i++)
    {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = vulkanHandler->GetTextureSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = (*descriptorSets)[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = (*descriptorSets)[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Renderer::SwapChainRecreated()
{
    CreateUniformBuffer();
    CreateDescriptorSets();
}