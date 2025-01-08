#pragma once
#include "Character/BehaviorTree/Node.h"

namespace BT
{
    class Inverter : public Node
    {
    public:
        Inverter(const std::wstring& kName);

        virtual Status TickNode(float delta_time) override;
    
    };
}
