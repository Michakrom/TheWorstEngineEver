#pragma once

#include "Component.h"

class TestComponent2 : public Component
{
    public:
        virtual void Update() override;

    private:
        float shift = 0;
};