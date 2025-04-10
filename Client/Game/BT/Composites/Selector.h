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

        static std::shared_ptr<Selector> Create(const std::wstring& kName);
    
    };
}
