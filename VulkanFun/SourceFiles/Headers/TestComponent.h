#pragma once
#include "Component.h"
#include "Vertex.h"

#include <vector>

class TestComponent : public Component
{
    public:
        virtual void Start() override;
        virtual void Update() override;
        virtual void Destroy() override;

    private:
        float shift = 0;
    std::vector<Vertex> vertices3 = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};
    std::vector<uint32_t> indices3 = {
        0, 1, 2, 2, 3, 0};
};