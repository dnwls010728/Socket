#pragma once
#include "BT/Node.h"

namespace BT
{
    class IStrategy;
}

namespace BT
{
    class Leaf : public Node
    {
    public:
        Leaf(const std::wstring& kName);
        virtual ~Leaf() override = default;

        virtual Status TickNode(float delta_time);

        virtual void Reset() override;

    protected:
        std::shared_ptr<IStrategy> strategy_;
    
    };
}
