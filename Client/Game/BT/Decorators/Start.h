#pragma once
#include "Decorator.h"

namespace BT
{
    class Start : public Decorator
    {
    public:
        Start(const std::wstring& kName);
        virtual ~Start() override = default;

        virtual Status TickNode(float delta_time) override;

        FORCEINLINE void SetRepeat(bool is_repeat) { is_repeat_ = is_repeat; }
        FORCEINLINE bool IsRepeat() const { return is_repeat_; }

        static std::shared_ptr<Start> Create(const std::wstring& kName);

    private:
        bool is_repeat_;
    
    };
}
