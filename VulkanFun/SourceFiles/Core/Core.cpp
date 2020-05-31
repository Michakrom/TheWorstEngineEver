#include "Core.h"
#include "InitialScene.h"

VulkanHandler Core::vulkanHandler;
std::unique_ptr<Scene> Core::currentScene;
std::unique_ptr<Scene> Core::queuedScene;

void Core::Run()
{
    InitWindow();
    vulkanHandler.InitVulkan(window);

    currentScene = std::make_unique<InitialScene>();
    currentScene->PrepareScene();
    Update();

    currentScene->Destroy();
    vulkanHandler.Cleanup();
}

VulkanHandler* Core::GetVulkanHandler()
{
    return &vulkanHandler;
}

Scene* Core::GetCurrentScene()
{
    return currentScene.get();
}

void Core::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "No name", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void Core::FramebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Core *>(glfwGetWindowUserPointer(window));
    app->vulkanHandler.MarkWindowAsResized();
}

void Core::Update()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        vulkanHandler.DrawFrame();

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