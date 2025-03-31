#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class BoomBase : public Actor
{
    SHADER_CLASS_HELPER(BoomBase)
    GENERATED_BODY(BoomBase, Actor)
    
public:
    BoomBase(const std::wstring& kName);
    virtual ~BoomBase() override = default;

    FORCEINLINE void SetDirection(const Math::Vector2& kDirection) { direction_ = kDirection; }
    FORCEINLINE const Math::Vector2& GetDirection() const { return direction_; }

    FORCEINLINE void SetLength(int kLength) { length_ = kLength; }
    FORCEINLINE int GetLength() const { return length_; }

protected:
    virtual void BeginPlay() override;

    void OnBoom();
    
#pragma region COMPONENTS
    class SpriteRendererComponent* renderer_;
    class BoxColliderComponent* collider_;
#pragma endregion

    Math::Vector2 direction_;
    
    int length_;

    TimerHandle timer_handle_;
    
};
