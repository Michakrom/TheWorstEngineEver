#include "TextRenderer.h"

void TextRenderer::SetPipeline()
{
    pipeline = vulkanHandler->GetTextGraphicsPipeline();
}