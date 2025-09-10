#pragma once
#include "NetworkActor.h"
#include "Subsystems/ObjectPool/IPoolable.h"

class Block : public NetworkActor, public IPoolable
{
    SHADER_CLASS_HELPER(Block)
    GENERATED_BODY(Block, NetworkActor)
    
public:
    Block(const std::wstring& name);
    virtual ~Block() override = default;

    void Init(const std::wstring& color, const Math::Vector2& position) const;

    virtual void OnActivate() override;
    virtual void OnDeactivate() override;

protected:
    virtual void OnEnable() override;
    virtual void OnDisable() override;

private:
    std::shared_ptr<SpriteRendererComponent> renderer_;
    
};
