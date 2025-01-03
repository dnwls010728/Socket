#pragma once
#include "Node.h"

namespace BT
{
    class UntilFall : public Node
    {
    public:
        UntilFall(const std::wstring& kName);
        virtual ~UntilFall() override = default;

        virtual Status Process() override;
    
    };
}
