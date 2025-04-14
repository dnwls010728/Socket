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
        Leaf(const std::wstring& kName, const std::shared_ptr<IStrategy>& strategy);
        virtual ~Leaf() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

        static std::shared_ptr<Leaf> Create(const std::wstring& kName, const std::shared_ptr<IStrategy>& strategy);

    protected:
        std::shared_ptr<IStrategy> strategy_;
    
    };
}
