#include "VulkanHandler.h"
#include "Renderer.h"
#include "Core.h"

// std::shared_ptr<VulkanHandler> app;
int main()
{
    try
    {
        Core app;
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
