#pragma once
#include "Node.h"

namespace BT
{
    class IStrategy
    {
    public:
        virtual Node::Status TickNode(float delta_time) = 0;
        
        FORCEINLINE virtual void Reset() {};
        
    };
}
