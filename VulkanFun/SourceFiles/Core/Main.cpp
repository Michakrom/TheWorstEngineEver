#include "VulkanHandler.h"
#include "Renderer.h"
#include "Core.h"

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
