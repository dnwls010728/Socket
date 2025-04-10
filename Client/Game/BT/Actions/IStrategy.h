#pragma once
#include "BT/Node.h"

namespace BT
{
    class Node;
    
    class IStrategy
    {
    public:
        virtual ~IStrategy() = default;

        virtual Node::Status TickNode(float delta_time) = 0;

        inline virtual void Reset() {}

    protected:
        IStrategy() = default;
    
    };
}
