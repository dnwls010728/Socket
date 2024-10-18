#pragma once
#include <map>
#include <memory>

#include "Actor/Component/ActorComponent.h"

class AnimationClip;
class SpriteRendererComponent;
class AnimationEvent;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    std::shared_ptr<AnimationClip> AddClip(const std::wstring& kName, int* sprite_idx_arr, int size);
    inline std::shared_ptr<AnimationClip> GetClip(std::wstring clip_name) { return clips_[clip_name]; }
    inline std::shared_ptr<AnimationClip> GetClip() { return current_clip_; }
    bool PlayClip(std::wstring clip_name);
    bool PlayClip(std::shared_ptr<AnimationClip> clip);
    inline bool IsPlaying() const { return is_playing_; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    SpriteRendererComponent* sprite_renderer_;

    std::map<std::wstring, std::shared_ptr<AnimationClip>> clips_;
    std::shared_ptr<AnimationClip> current_clip_;

    float timer_;
    int current_index_;
    bool is_playing_;
};