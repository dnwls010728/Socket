#pragma once
#include "Node.h"

namespace BT
{
    class Selector : public Node
    {
    public:
        Selector(const std::wstring& kName);
        virtual ~Selector() override = default;

        virtual Status TickNode(float delta_time) override;
    };
}
