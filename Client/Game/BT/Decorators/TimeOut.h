#pragma once
#include "Decorator.h"

namespace BT
{
    class TimeOut : public Decorator
    {
    public:
        TimeOut(const std::wstring& kName, float duration);
        virtual ~TimeOut() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

        static std::shared_ptr<TimeOut> Create(const std::wstring& kName, float duration);

    private:
        bool is_started_;
        
        float duration_;
        float timer_;
    
    };
}
