#include "TextMaker.h"
#include "iostream"
#include <vector>
#include "Vertex.h"

FT_Library TextMaker::library;
FT_Face TextMaker::face;

void TextMaker::Initialize()
{
    bool error = FT_Init_FreeType(&library);
    if (error)
    {
        throw std::runtime_error("FreeType: Unable to initialize library");
    }

    error = FT_New_Face(library,
                        "C:/Users/Admin/workspace/C++/VulkanFun/VulkanFun/"
                        "Fonts/arial.ttf",
                        0,
                        &face);
    if (error == FT_Err_Unknown_File_Format)
    {
        throw std::runtime_error("FreeType: Unsupported font format");
    }
    else if (error)
    {
        throw std::runtime_error("FreeType: Unknown font loading error");
    }
}

FT_Bitmap TextMaker::GetCharImage(char character)
{
    FT_GlyphSlot slot = face->glyph;

    bool error = FT_Set_Pixel_Sizes(
        face,
        512,
        0); 

    error = FT_Load_Char(face, character, FT_LOAD_RENDER);

    if (error)
        throw std::runtime_error("FreeType: Unable to load character");

    return slot->bitmap;;
}