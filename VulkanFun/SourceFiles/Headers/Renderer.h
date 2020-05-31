#pragma once
#include "Vertex.h"
#include "VulkanHandler.h"
#include "Texture.h"
#include "Component.h"
#include "Transform.h"

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

class Renderer : public Component, public std::enable_shared_from_this<Renderer>
{
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void Destroy() override;

    void Initialize();
    void Free();

    std::vector<Vertex> *GetVertices() const;
    void SetVertices(std::shared_ptr<std::vector<Vertex>> vertices);

    std::vector<uint32_t> *GetIndices() const;
    void SetIndices(std::shared_ptr<std::vector<uint32_t>> indices);

    std::vector<VkDescriptorSet> *GetDescriptorSets() const;

    std::shared_ptr<Texture> GetTexture() const;
    void SetTexture(std::shared_ptr<Texture> texture);

    VkBuffer GetVertexBuffer() const;
    VkBuffer GetIndexBuffer() const;

    VkPipeline GetPipeline() const;

    void CreateDescriptorSets();
    void SwapChainRecreated();

    virtual ~Renderer();

protected:
    VulkanHandler *vulkanHandler;
    VkPipeline pipeline;

    virtual void SetPipeline();

private:
    std::shared_ptr<std::vector<Vertex>> vertices;
    std::shared_ptr<std::vector<uint32_t>> indices;
    std::shared_ptr<Texture> texture;
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
    std::weak_ptr<Transform> transform;

    bool destroyed = false;

    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffer();
    void CreateTextureImageView();
    void CreateTextureImage();
    void DestroyUniformBuffers();
    void UpdateUniformBuffer();
};