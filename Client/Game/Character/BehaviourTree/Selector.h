#pragma once
#include "Node.h"

namespace BT
{
    class Selector : public Node
    {
        Selector(const std::wstring& kName);
        virtual ~Selector() override = default;

        virtual Status Process() override;
    };
}
