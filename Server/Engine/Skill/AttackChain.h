#pragma once
#include <vector>
#include <memory>

#include "AttackEffect.h"
#include "DataManager.h"

class AttackChain
{
public:
    AttackChain() = default;
    ~AttackChain() = default;
    AttackChain(const HitFrame& hit_frame);
    AttackChain(AttackChain&& other) noexcept;
    AttackChain& operator=(AttackChain&& other) noexcept;
    AttackChain(const AttackChain&)            = delete;
    AttackChain& operator=(const AttackChain&) = delete;

    void AddChain(std::shared_ptr<AttackEffect> attack_effect);
    void AddChain(const HitFrame& hit_frame);
    void SetChain(const HitFrame& hit_frame);
    void ClearChain();

    void Apply(const AttackEffect::AttackContext& ctx);
    
private:
    std::vector<std::shared_ptr<AttackEffect>> attack_chain_;
};
