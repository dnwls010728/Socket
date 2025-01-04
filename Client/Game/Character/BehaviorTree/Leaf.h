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
        Leaf(const std::wstring& kName, const std::shared_ptr<IStrategy>& strategy);
        virtual ~Leaf() override = default;

        virtual void Reset() override;
        virtual Status TickNode(float delta_time) override;

    protected:
        std::shared_ptr<IStrategy> strategy_;
    
    };
}
