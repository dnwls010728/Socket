#include "pch.h"
#include "AttackChain.h"

#include "AttackEffect.h"
#include "DataManager.h"
#include "AttackEffect/DamageEffect.h"

AttackChain::AttackChain(AttackChain&& other) noexcept : attack_chain_(std::move(other.attack_chain_)) {}

AttackChain::AttackChain(const HitFrame& hit_frame)
{
    SetChain(hit_frame);
}

AttackChain& AttackChain::operator=(AttackChain&& other) noexcept
{
    if (this != &other) {
        attack_chain_ = std::move(other.attack_chain_);
    }
    return *this;
}

void AttackChain::AddChain(std::shared_ptr<AttackEffect> attack_effect)
{
    attack_chain_.push_back(attack_effect);
}

void AttackChain::AddChain(const HitFrame& hit_frame)
{
    auto effect = std::make_shared<DamageEffect>();
    effect->SetDamageMultiplier(hit_frame.damage / 100.f);
    effect->SetBaseDamage(0);
    effect->SetAttackCount(hit_frame.hit_count);

    attack_chain_.push_back(effect);
}

void AttackChain::SetChain(const HitFrame& hit_frame)
{
    ClearChain();
    AddChain(hit_frame);
}

void AttackChain::ClearChain()
{
    attack_chain_.clear();
}

void AttackChain::Apply(const AttackEffect::AttackContext& ctx)
{
    for (auto &effect : attack_chain_)
        effect->Apply(ctx);
}
