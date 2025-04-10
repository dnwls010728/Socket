#pragma once
#include "Composite.h"

namespace BT
{
    class Selector : public Composite
    {
    public:
        Selector(const std::wstring& kName);
        virtual ~Selector() override = default;

        virtual Status TickNode(float delta_time) override;
    
    };
}
