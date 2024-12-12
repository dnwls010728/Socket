#pragma once
#include "Asset.h"

class AnimationClip : public Asset
{
    GENERATED_BODY(AnimationClip, Asset)

public:
    AnimationClip();
    virtual ~AnimationClip() override = default;
    
    virtual bool Load(const std::wstring& kPath) override;

private:
    YAML::Node data_;
    
};
