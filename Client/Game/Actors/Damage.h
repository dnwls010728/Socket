#pragma once

class DamageRendererComponent;

class Damage : public Actor
{
    SHADER_CLASS_HELPER(Damage)
    GENERATED_BODY(Damage, Actor)
    
public:
    Damage(const std::wstring& name);
    virtual ~Damage() override = default;

protected:
    virtual void Tick(float delta_time) override;

private:
    std::shared_ptr<DamageRendererComponent> damage_renderer_;

    float fade_timer_;
    
};
