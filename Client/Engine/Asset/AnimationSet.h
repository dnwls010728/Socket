#pragma once
#include "Asset.h"

struct AnimationClip
{
};

class AnimationSet : public Asset
{
    GENERATED_BODY(AnimationSet, Asset)

public:
    AnimationSet();
    virtual ~AnimationSet() override = default;
    
    virtual bool Load(const std::wstring& kPath) override;

private:
    YAML::Node data_;

    std::map<std::wstring, AnimationClip> clips_;
    
};
