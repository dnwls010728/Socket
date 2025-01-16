#include "pch.h"
#include "Weapon.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "ObjectPool/Bullet.h"
#include "ObjectPool/ObjectPoolComponent.h"
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

    bullet_pool_ = AddComponent<ObjectPoolComponent>(L"BulletPool");
    bullet_pool_->SetPooledObjectClass(Bullet::StaticClass());

    GetTransform()->SetScale({.75f, .75f});
}

void Weapon::Shot(const Math::Vector2& kDirection)
{
    Math::Vector2 position = GetTransform()->GetPosition();
    
    float theta = std::atan2f(kDirection.y, kDirection.x);
    
    PooledObject* bullet = bullet_pool_->SpawnPooledObject();
    if (bullet)
    {
        bullet->SetOwner(this);
        bullet->SetInstigator(GetOwner());
        bullet->GetTransform()->SetPosition(position + kDirection * .5f);
        bullet->GetTransform()->SetAngle(theta * Math::Rad2Deg());
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Weapon>("Weapon")
        .constructor<const std::wstring&>();
}
