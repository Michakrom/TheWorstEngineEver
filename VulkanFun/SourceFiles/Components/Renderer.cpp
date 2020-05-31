#include "Renderer.h"
#include "TextMaker.h"
#include "Core.h"

#include <glm/glm.hpp>
#include <stb_image.h>

void Renderer::Start()
{
    vulkanHandler = Core::GetVulkanHandler();
    device = vulkanHandler->GetDevice();
    SetPipeline();
    transform = gameObject.lock()->GetComponent<Transform>();
}

void Renderer::Update()
{
    UpdateUniformBuffer();
}

void Renderer::Destroy()
{
    vulkanHandler->RemoveRenderer(shared_from_this());
    destroyed = true;
}

void Renderer::Initialize()
{
    CreateTextureImage();
    CreateTextureImageView();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffer();
    vulkanHandler->RagisterRenderer(shared_from_this());
    Update();
}

void Renderer::Free()
{
    DestroyUniformBuffers();

    vkDestroyImageView(device, textureImageView, nullptr);

    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
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

VkBuffer Renderer::GetVertexBuffer() const
{
    return vertexBuffer;
}
VkBuffer Renderer::GetIndexBuffer() const
{
    return indexBuffer;
}

VkPipeline Renderer::GetPipeline() const
{
    return pipeline;
}

Renderer::~Renderer() { }

void Renderer::SetPipeline()
{
    pipeline = vulkanHandler->GetStandardGraphicsPipeline();
}

void Renderer::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof((*vertices)[0]) * vertices->size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    vulkanHandler->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory,
                0, bufferSize, 0, &data);
    memcpy(data, vertices->data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    vulkanHandler->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    vulkanHandler->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof((*indices)[0]) * indices->size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    vulkanHandler->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0,
                bufferSize, 0, &data);
    memcpy(data, indices->data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    vulkanHandler->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

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
        vulkanHandler->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

void Renderer::CreateTextureImageView()
{
    textureImageView = vulkanHandler->createImageView(
        textureImage, texture->format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Renderer::CreateTextureImage()
{
    unsigned char *pixels = texture->buffer.get();

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
    vulkanHandler->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    vulkanHandler->createImage(texture->width, texture->height, texture->format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    vulkanHandler->transitionImageLayout(textureImage, texture->format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vulkanHandler->CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texture->width), static_cast<uint32_t>(texture->height));
    vulkanHandler->transitionImageLayout(textureImage, texture->format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(
        vulkanHandler->GetSwapChainImages().size(),
        vulkanHandler->GetDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = vulkanHandler->GetDescriptorPool();
    allocInfo.descriptorSetCount =
        static_cast<uint32_t>(vulkanHandler->GetSwapChainImages().size());
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
    SetPipeline();
    DestroyUniformBuffers();
    CreateUniformBuffer(); 
    CreateDescriptorSets();
}

void Renderer::DestroyUniformBuffers()
{
    for (size_t i = 0; i < vulkanHandler->GetSwapChainImages().size();
         i++)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
}

void Renderer::UpdateUniformBuffer()
{
    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);

    glm::vec3 pos = transform.lock()->GetPosition();
    glm::vec3 rotation = transform.lock()->GetRotation();
    ubo.view = glm::lookAt(
        glm::vec3(-pos.x, -pos.y, 3.0f),
        glm::vec3(-pos.x, -pos.y, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        vulkanHandler->GetSwapChainExtent().width /
            (float)vulkanHandler->GetSwapChainExtent().height,
        0.1f,
        10.0f
    );
    ubo.proj[1][1] *= -1;

    int curentImageIndex = vulkanHandler->GetCurrentImageIndex();
    void *data;
    vkMapMemory(device, uniformBuffersMemory[curentImageIndex], 0,
        sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[curentImageIndex]);
}