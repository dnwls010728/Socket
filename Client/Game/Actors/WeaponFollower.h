#pragma once

class WeaponFollower : public Actor
{
    SHADER_CLASS_HELPER(WeaponFollower)
    GENERATED_BODY(WeaponFollower, Actor)
    
public:
    WeaponFollower(const std::wstring& name);
    virtual ~WeaponFollower() override = default;
    
};
