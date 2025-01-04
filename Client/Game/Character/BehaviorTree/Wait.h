#pragma once
#include "Node.h"

namespace BT
{
    class Wait : public Node
    {
    public:
        Wait(const std::wstring& kName, float duration);
        virtual ~Wait() override = default;

        virtual Status Process() override;

        virtual void Reset() override;

    private:
        float duration_;
        float start_time_;
    
    };
}
