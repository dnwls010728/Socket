#pragma once
#include "Asset/Asset.h"

class Animation;

class AnimationPack : public Asset
{
    GENERATED_BODY(AnimationPack, Asset)
    
public:
    AnimationPack();
    virtual ~AnimationPack() override = default;

    virtual bool Load(const std::wstring& kPath) override;

private:
    std::wstring target_;
    
    std::map<std::wstring, std::shared_ptr<Animation>> animations_;
    
};
