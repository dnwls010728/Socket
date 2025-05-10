#pragma once
#include "Decorator.h"

namespace BT
{
    class Repeat : public Decorator
    {
    public:
        Repeat(const std::wstring& kName, int32_t count = 0);
        virtual ~Repeat() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

        static std::shared_ptr<Repeat> Create(const std::wstring& kName, int32_t count = 0);

    private:
        int32_t count_;
        int32_t counter_;
    
    };
}
