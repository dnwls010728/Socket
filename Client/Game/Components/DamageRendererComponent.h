#pragma once
#include "Math/Color.h"

class Sprite;

class DamageRendererComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(DamageRendererComponent)
    GENERATED_BODY(DamageRendererComponent, ActorComponent)
    
public:
    DamageRendererComponent(Actor* owner, const std::wstring& name);
    virtual ~DamageRendererComponent() override = default;
    
    void SetDamage(uint64_t damage);
    void SetColor(const Math::Color& color);

    FORCEINLINE void SetDamageSprite(Sprite* sprite) { damage_sprite_ = sprite; }
    FORCEINLINE void SetMissSprite(Sprite* sprite) { miss_sprite_ = sprite; }

    FORCEINLINE Math::Color GetColor() const { return color_; }

protected:
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void TickComponent(float delta_time) override;
    virtual void Render(float alpha) override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

private:
    void RebuildGridGeometry();

    uint64_t damage_;

    std::vector<DefaultVertex> vertices_;
    std::vector<uint32_t> indices_;

    std::shared_ptr<Shape> shape_;

    Sprite* damage_sprite_;
    Sprite* miss_sprite_;
    
    Math::Color color_;

    bool is_dirty_;
    
};
