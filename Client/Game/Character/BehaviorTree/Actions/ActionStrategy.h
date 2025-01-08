#pragma once
#include "IStrategy.h"
#include "Misc/Function.h"

namespace BT
{
    class ActionStrategy : public IStrategy
    {
    public:
        ActionStrategy(const Function<void(void)>& kFunc);

        virtual Node::Status TickNode(float delta_time) override;

    private:
        Function<void(void)> func_;
    
    };
}
