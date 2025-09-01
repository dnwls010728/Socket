#pragma once
#include <optional>

#include "ActorComponent.h"
#include "Math/Bounds.h"
#include "Math/Color.h"
#include "Windows/DX/Sprite.h"

class Sprite;

class SpriteRendererComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(SpriteRendererComponent)
    GENERATED_BODY(SpriteRendererComponent, ActorComponent)
    
public:
    SpriteRendererComponent(Actor* owner, const std::wstring& kName = L"");
    virtual ~SpriteRendererComponent() override = default;

    void SetColor(const Math::Color& color);
    void SetZOrder(int32_t z_order);
    void SetSprite(Sprite* sprite, const std::wstring& kFrame);
    void SetSprite(Sprite* sprite, uint64_t frame_index = 0);

    Bounds GetBounds() const;

    FORCEINLINE Sprite* GetSprite() const { return sprite_; }

    FORCEINLINE void SetFlipX(bool flip_x) { flip_x_ = flip_x; }
    FORCEINLINE bool IsFlipX() const { return flip_x_; }

    FORCEINLINE void SetFlipY(bool flip_y) { flip_y_ = flip_y; }
    FORCEINLINE bool IsFlipY() const { return flip_y_; }

    FORCEINLINE Math::Color GetColor() const { return color_; }

    FORCEINLINE int32_t GetZOrder() const { return z_order_; }

protected:
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void Render(float alpha) override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

private:
    void UpdateFrame();
    
    std::shared_ptr<class Shape> shape_;
    
    Sprite* sprite_;

    uint64_t frame_index_;

    std::optional<SpriteFrame> frame_;

    float frame_width_;
    float frame_height_;

    bool flip_x_;
    bool flip_y_;

    Math::Color color_;

    int32_t z_order_;
    
};
