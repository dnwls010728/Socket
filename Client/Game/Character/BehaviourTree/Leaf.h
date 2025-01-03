#pragma once
#include "Node.h"

namespace BT
{
    class IStrategy;
}

namespace BT
{
    class Leaf : public Node
    {
    public:
        Leaf(const std::wstring& kName, IStrategy* strategy);
        virtual ~Leaf() override = default;

        virtual void Reset() override;
        virtual Status Process() override;

    protected:
        IStrategy* strategy_;
    
    };
}
