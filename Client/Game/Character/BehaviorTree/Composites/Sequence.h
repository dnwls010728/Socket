#pragma once
#include "Character/BehaviorTree/Node.h"

namespace BT
{
    class Sequence : public Node
    {
    public:
        Sequence(const std::wstring& kName);
        virtual ~Sequence() override = default;

        virtual Status TickNode(float delta_time) override;
        
    };
}
