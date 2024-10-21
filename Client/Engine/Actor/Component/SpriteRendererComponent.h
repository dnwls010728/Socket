#pragma once
#include "ActorComponent.h"
#include "Math/Color.h"

class Sprite;

class SpriteRendererComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(SpriteRendererComponent)
    GENERATED_BODY(SpriteRendererComponent, ActorComponent)
    
public:
    SpriteRendererComponent(Actor* owner, const std::wstring& kName);
    virtual ~SpriteRendererComponent() override = default;

    inline void SetSprite(Sprite* sprite) { sprite_ = sprite; }
    inline const Sprite* GetSprite() const { return sprite_; }

    inline void SetFrameIndex(Type::uint32 frame_index) { frame_index_ = frame_index; }
    inline Type::uint32 GetFrameIndex() const { return frame_index_; }

    inline void SetFlipX(bool flip_x) { flip_x_ = flip_x; }
    inline bool GetFlipX() const { return flip_x_; }

    inline void IsFlipX(bool flip_y) { flip_y_ = flip_y; }
    inline bool IsFlipY() const { return flip_y_; }

    inline void SetColor(const Math::Color& color) { color_ = color; }
    inline Math::Color GetColor() const { return color_; }

protected:
    virtual void InitializeComponent() override;
    virtual void Render(float alpha) override;

private:
    friend class AnimatorComponent;
    
    std::shared_ptr<class Shape> shape_;
    Sprite* sprite_;

    Type::uint32 frame_index_;

    bool flip_x_;
    bool flip_y_;

    Math::Color color_;
    
};
