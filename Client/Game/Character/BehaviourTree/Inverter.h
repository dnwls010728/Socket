#pragma once
#include "Node.h"

namespace BT
{
    class Inverter : public Node
    {
    public:
        Inverter(const std::wstring& kName);

        virtual Status Process() override;
    
    };
}
