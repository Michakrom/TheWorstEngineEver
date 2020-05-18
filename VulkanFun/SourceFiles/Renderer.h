#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "VulkanHandler.h"
#include <memory>
#include "Texture.h"
#include "Component.h"

class Renderer : public Component, public std::enable_shared_from_this<Renderer>
{
private:
    std::shared_ptr<std::vector<Vertex>> vertices;
    std::shared_ptr<std::vector<uint32_t>> indices;
    std::shared_ptr<Texture> texture;
    VulkanHandler* vulkanHandler;
    VkImage textureImage;
    VkImageView textureImageView;

    std::unique_ptr<std::vector<VkDescriptorSet>> descriptorSets;
    std::vector<VkBuffer> uniformBuffers;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkDeviceMemory textureImageMemory;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    VkDevice device;

public:
    virtual void Start();
    virtual void Update();
    virtual void Destroy();

    Renderer();
    Renderer(const std::shared_ptr<std::vector<Vertex>> vertices,
             std::unique_ptr<std::vector<uint32_t>> indices,
             const std::shared_ptr<Texture> texture);
    Renderer(const Renderer &rend);
    ~Renderer();

    float shift;

    void Initialize();

    std::vector<Vertex> *GetVertices() const;
    void SetVertices(std::shared_ptr<std::vector<Vertex>> vertices);

    std::vector<uint32_t> *GetIndices() const;
    void SetIndices(std::shared_ptr<std::vector<uint32_t>> indices);

    std::vector<VkDescriptorSet> *GetDescriptorSets() const;
    void SetDescriptorSets(
        std::unique_ptr<std::vector<VkDescriptorSet>> descriptorSets);

    std::shared_ptr<Texture> GetTexture() const;
    void SetTexture(std::shared_ptr<Texture> texture);

    VkBuffer GetVertexBuffer() const;
    VkBuffer GetIndexBuffer() const;

    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffer();
    void CreateTextureImageView();
    void CreateTextureImage();
    void CreateDescriptorSets();

    void SwapChainRecreated();
};