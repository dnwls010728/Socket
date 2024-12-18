#include "pch.h"
#include "Weapon.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/ObjectPool/Bullet.h"
#include "Actor/ObjectPool/ObjectPool.h"
#include "Asset/AssetManager.h"
#include "Input/Mouse.h"
#include "Math/Math.h"
#include "Windows/DX/Sprite.h"

Weapon::Weapon(const std::wstring& kName) :
    Actor(kName)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");
    
    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Weapon\\Icon29_12.png");
    if (sprite_) renderer_->SetSprite(sprite_, L"Icon29_12_0");

    bullet_pool_ = AddComponent<ObjectPool>(L"Bullet Pool");
    bullet_pool_->SetPooledObjectClass(Bullet::StaticClass());
}

void Weapon::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
    Math::Vector2 position = GetTransform()->GetPosition();
    
    Mouse* mouse = Mouse::Get();
    Math::Vector2 mouse_position = Renderer::Get()->ConvertScreenToWorld(mouse->GetMousePosition());
    Math::Vector2 direction = (mouse_position - position).Normalized();
        
    renderer_->SetFlipX(direction.x < 0.f);

    float theta = std::atan2f(direction.y, direction.x);
    
    float degree;
    if (direction.x < 0.f)
    {
        degree = theta * (180.f / MATH_PI) - 135.f;
    }
    else
    {
        degree = theta * (180.f / MATH_PI) - 45.f;
    }

    GetTransform()->SetAngle(degree);

    if (mouse->GetMouseButtonDown(MouseButton::kLeft))
    {
        PooledObject* bullet = bullet_pool_->SpawnPooledObject();
        if (bullet)
        {
            bullet->GetTransform()->SetPosition(position + direction * .5f);
            bullet->GetTransform()->SetAngle(theta * (180.f / MATH_PI));
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Weapon>("Weapon")
        .constructor<const std::wstring&>();
}
