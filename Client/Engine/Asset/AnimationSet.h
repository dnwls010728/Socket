#pragma once
#include "Asset.h"

struct AnimationSequence
{
    std::wstring name;
    int sample_frame_rate;
    bool is_loop;
    std::vector<std::wstring> frames;
};

class AnimationSet : public Asset
{
    GENERATED_BODY(AnimationSet, Asset)

public:
    AnimationSet();
    virtual ~AnimationSet() override = default;
    
    virtual bool Load(const std::wstring& kPath) override;

    FORCEINLINE const std::wstring& GetTarget() const { return target_; }
    FORCEINLINE const std::map<std::wstring, AnimationSequence>& GetSequences() const { return sequences_; }

private:
    YAML::Node data_;
    std::wstring target_;
    std::map<std::wstring, AnimationSequence> sequences_;
    
};
