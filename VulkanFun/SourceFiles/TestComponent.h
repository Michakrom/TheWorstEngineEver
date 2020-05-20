#pragma once

#include "Component.h"

class TestComponent : public Component
{
    public:
        virtual void Start() override;
        virtual void Update() override;
        virtual void Destroy() override;

    private:
        float shift = 0;
};