#include "Core.h"
#include "InitialScene.h"

VulkanHandler Core::vulkanHandler;
std::unique_ptr<Scene> Core::currentScene;
std::unique_ptr<Scene> Core::queuedScene;

void Core::Run()
{
    initWindow();
    vulkanHandler.initVulkan(window);

    currentScene = std::make_unique<InitialScene>();
    currentScene->PrepareScene();
    Update();

    currentScene->Destroy();
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

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "No name", nullptr, nullptr);
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

        if (queuedScene)
            LoadScene();
    }
    vulkanHandler.WaitIdle();
}

void Core::LoadScene()
{
    currentScene->Destroy();
    currentScene = std::move(queuedScene);
    currentScene->PrepareScene();
}