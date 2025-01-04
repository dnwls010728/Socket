#pragma once
#include "Node.h"

namespace BT
{
    class BehaviorTree : public Node
    {
    public:
        BehaviorTree(const std::wstring& kName);
        virtual ~BehaviorTree() override = default;

        virtual Status TickNode(float delta_time) override;
        
    };
}
