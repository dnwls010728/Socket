#pragma once
#include "Actor/Actor.h"

class NetworkActor : public Actor
{
    SHADER_CLASS_HELPER(NetworkActor)
    GENERATED_BODY(NetworkActor, Actor)
    
public:
    NetworkActor(const std::wstring& kName);
    virtual ~NetworkActor() override = default;

    FORCEINLINE void SetIsMine(bool is_mine) { is_mine_ = is_mine; }
    FORCEINLINE bool IsMine() const { return is_mine_; }

protected:
    bool is_mine_;
    
};
