#pragma once

class Portal : public Actor
{
    SHADER_CLASS_HELPER(Portal)
    GENERATED_BODY(Portal, Actor)
    
public:
    Portal(const std::wstring& name);
    virtual ~Portal() override = default;

    FORCEINLINE void SetID(uint32_t id) { id_ = id; }

private:
    int32_t id_;
    
};
