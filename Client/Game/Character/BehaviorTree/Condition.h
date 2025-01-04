#pragma once
#include "IStrategy.h"
#include "Misc/Function.h"

namespace BT
{
    class Condition : public IStrategy
    {
    public:
        Condition(const Function<bool(void)>& kFunc);
        ~Condition() = default;

        virtual Node::Status Process() override;
    
    private:
        Function<bool(void)> func_;
    
    };
}
