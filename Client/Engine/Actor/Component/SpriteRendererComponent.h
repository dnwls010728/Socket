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

    void SetZOrder(int z_order);
    void SetSprite(Sprite* sprite);
    void SetFrame(const std::wstring& kFrame);

    FORCEINLINE Sprite* GetSprite() const { return sprite_; }

    FORCEINLINE void SetFlipX(bool flip_x) { flip_x_ = flip_x; }
    FORCEINLINE bool GetFlipX() const { return flip_x_; }

    FORCEINLINE void IsFlipX(bool flip_y) { flip_y_ = flip_y; }
    FORCEINLINE bool IsFlipY() const { return flip_y_; }

    FORCEINLINE void SetColor(const Math::Color& color) { color_ = color; }
    FORCEINLINE Math::Color GetColor() const { return color_; }

    FORCEINLINE int GetZOrder() const { return z_order_; }

protected:
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void Render(float alpha) override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

private:
    std::shared_ptr<class Shape> shape_;
    Sprite* sprite_;

    std::wstring current_frame_;

    bool flip_x_;
    bool flip_y_;

    Math::Color color_;

    int z_order_;
    
};
