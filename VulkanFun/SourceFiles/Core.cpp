#include "Core.h"

VulkanHandler Core::vulkanHandler;
std::unique_ptr<Scene> Core::currentScene;

void Core::Run()
{
    initWindow();
    vulkanHandler.initVulkan(window);
    currentScene = std::make_unique<Scene>();
    currentScene->PrepareScene();
    Update();
    //
    currentScene->Destroy();
    //
    vulkanHandler.cleanup();
}

VulkanHandler* Core::GetVulkanHandler()
{
    return &vulkanHandler;
}

Scene* Core::GetCurrentScene()
{
    return currentScene.get();
}

void Core::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "No name", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Core::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Core *>(glfwGetWindowUserPointer(window));
    app->vulkanHandler.MarkWindowAsResized();
}

void Core::Update()
{
    while (!glfwWindowShouldClose(window))
    {
        currentScene->UpdateGameObjects();
        glfwPollEvents();
        vulkanHandler.drawFrame();
    }
    vulkanHandler.WaitForDrawEnd();
}