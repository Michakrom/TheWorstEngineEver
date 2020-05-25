#pragma once
#include <vector>
#include "Vertex.h"

class ModelLoader
{
public:
    static void LoadModelAtPath(const std::string path,
                                std::vector<Vertex> *outVertices,
                                std::vector<uint32_t> *outIndices);
};