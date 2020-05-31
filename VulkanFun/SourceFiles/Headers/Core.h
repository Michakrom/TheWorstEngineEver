#pragma once;

#include "VulkanHandler.h"
#include "Scene.h"

#include <memory>
#include <GLFW/glfw3.h>

class Core
{
public:
    static VulkanHandler* GetVulkanHandler();
    static Scene* GetCurrentScene();
    void Run();
    void InitWindow();
    static void FramebufferResizeCallback(
        GLFWwindow *window, int width, int height
    );

    template <typename T>
    static void SwitchScene()
    {
        queuedScene = std::make_unique<T>();
    }

private:
    static VulkanHandler vulkanHandler;
    static std::unique_ptr<Scene> currentScene;
    static std::unique_ptr<Scene> queuedScene;

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    GLFWwindow *window;

    void Update();
    void LoadScene();
};