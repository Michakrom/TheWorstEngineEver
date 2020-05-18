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
    void initWindow();
    static void framebufferResizeCallback(
        GLFWwindow *window, int width, int height
    );

private:
    static VulkanHandler vulkanHandler;
    static std::unique_ptr<Scene> currentScene;

    const int WIDTH = 800;
    const int HEIGHT = 600;

    GLFWwindow *window;

    void Update();
};