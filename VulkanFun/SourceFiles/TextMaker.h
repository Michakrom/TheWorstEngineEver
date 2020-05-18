#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class TextMaker
{
public:
    static void Initialize();
    static FT_Bitmap GetCharImage(char character);

private:
    static FT_Library library;
    static FT_Face face;
};
