#pragma once
#include "Node.h"

namespace BT
{
    class BehaviourTree : public Node
    {
    public:
        BehaviourTree(const std::wstring& kName);
        virtual ~BehaviourTree() override = default;

        virtual Status Process() override;
        
    };
}
