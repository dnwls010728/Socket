#pragma once
#include "Node.h"
#include "Misc/Function.h"

namespace BT
{
    class Abort : public Node
    {
    public:
        Abort(const std::wstring& kName, const Function<bool(void)>& kFunc);
        virtual ~Abort() override = default;

        virtual Status TickNode(float delta_time) override;

    private:
        Function<bool(void)> func_;
    
    };
}
