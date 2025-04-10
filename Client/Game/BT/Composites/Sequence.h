#pragma once
#include "Composite.h"

namespace BT
{
    class Sequence : public Composite
    {
    public:
        Sequence(const std::wstring& kName);
        virtual ~Sequence() override = default;

        virtual Status TickNode(float delta_time) override;
    
    };
}
