#pragma once
#include "Decorator.h"

namespace BT
{
    class Repeat : public Decorator
    {
    public:
        Repeat(const std::wstring& kName, int count = 0);
        virtual ~Repeat() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        int count_;
        int counter_;
    
    };
}
