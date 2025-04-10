#pragma once
#include "Decorator.h"

namespace BT
{
    class Inverter : public Decorator
    {
    public:
        Inverter(const std::wstring& kName);
        virtual ~Inverter() override = default;

        virtual Status TickNode(float delta_time) override;
    
    };
}
