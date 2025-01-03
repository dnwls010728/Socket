#pragma once
#include "Node.h"

namespace BT
{
    class IStrategy
    {
    public:
        virtual Node::Status Process() = 0;
        virtual void Reset() = 0;
        
    };
}
