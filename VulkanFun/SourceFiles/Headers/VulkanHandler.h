#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>
#include <map>

#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"
#include "Vertex.h"
#include "hash.h"
#include "UniformBufferObject.h"

class Renderer;

class VulkanHandler
{
public:
    void InitVulkan(GLFWwindow *window);
    VkDevice GetDevice();
    std::vector<VkImage> &GetSwapChainImages();
    VkExtent2D &GetSwapChainExtent();
    VkDescriptorSetLayout GetDescriptorSetLayout();
    VkDescriptorPool GetDescriptorPool();
    VkSampler GetTextureSampler();
    VkPipeline GetStandardGraphicsPipeline();
    VkPipeline GetTextGraphicsPipeline();
    uint32_t GetCurrentImageIndex();
    void RagisterRenderer(std::shared_ptr<Renderer> renderer);
    void RemoveRenderer(std::shared_ptr<Renderer> renderer);
    void MarkWindowAsResized();
    void WaitIdle();
    void ResetRenderers();
    void DrawFrame();
    void Cleanup();
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags);
    void createImage(uint32_t width, uint32_t height, VkFormat format, 
        VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties, VkImage &image,
        VkDeviceMemory &imageMemory
    );
    void transitionImageLayout(VkImage image, VkFormat format,
        VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, 
        uint32_t height);

private:
    const int WIDTH = 800;
    const int HEIGHT = 600;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    GLFWwindow *window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline standardGraphicsPipeline;
    VkPipeline textGraphicsPipeline;

    VkCommandPool commandPool;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    std::list<std::shared_ptr<Renderer>> renderers;
    std::multimap<size_t, std::shared_ptr<Renderer>> renderersToFree;

    VkDescriptorPool descriptorPool;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
    uint32_t imageIndex = 0;

    bool frameBufferResized = false;
    bool rendererVectorChanged = false;

    static void FramebufferResizeCallback(GLFWwindow *window, int width,
        int height
    );
    void CleanupSwapChain();
    void CreateInstance(const char *applicationName, const char *engineName);
    void PopulateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT &createInfo
    );
    void SetupDebugMessenger();
    void CreateSurface();
    void RecreateSwapChain();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipelineLayout();
    void CreateAllGraphicsPipelines();
    void DestroyAllGraphicsPipelines();
    void CreateGraphicsPipeline(bool createRegular, VkPipeline& targetPipeline);
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateDepthResources();
    VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates,   
        VkImageTiling tiling, VkFormatFeatureFlags features
    );
    VkFormat FindDepthFormat();
    bool HasStencilComponent(VkFormat format);
    void CreateTextureSampler();
    void CreateDescriptorPool();
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    uint32_t FindMemoryType(uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    );
    void CreateCommandBuffers();
    void CreateSyncObjects();
    VkShaderModule CreateShaderModule(const std::vector<char> &code);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats
    );
    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes
    );
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    std::vector<const char *> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );
    void FreeRenderers();
};