#pragma once

class Animation
{
public:
    Animation();
    ~Animation() = default;

private:
    friend class AnimationPack;
    friend class AnimatorComponent;
    
    int frame_rate_;
    bool is_loop_;
    std::vector<std::wstring> frames_;
    
};
