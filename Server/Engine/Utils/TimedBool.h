#pragma once

class TimedBool
{
public:
    TimedBool();
    ~TimedBool() = default;

    FORCEINLINE explicit operator bool() const { return value_; }

    FORCEINLINE bool operator==(bool value) const { return value_ == value; }
    FORCEINLINE bool operator!=(bool value) const { return value_ != value; }

    TimedBool& operator=(bool value);
    
    void Set(float seconds);
    void Tick(float delta_time);

    float GetAlpha() const;

private:
    float duration_;
    float remaining_;

    bool value_;
    
};
