#pragma once

class Animation
{
public:
    Animation(const std::wstring& kName);
    ~Animation() = default;

    FORCEINLINE const std::wstring& GetName() const { return name_; }

private:
    friend class AnimationPack;
    friend class AnimatorComponent;

    std::wstring name_;
    int32_t frame_rate_;
    bool is_loop_;
    std::vector<std::wstring> frames_;
    
};
