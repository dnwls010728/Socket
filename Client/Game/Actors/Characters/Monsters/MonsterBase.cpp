#include "pch.h"
 #include "MonsterBase.h"
 
 #include <CustomPacket.h>
 
 #include "Actor/Component/SpriteRendererComponent.h"
 #include "Actor/Component/TransformComponent.h"
#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/Components/Controller2DComponent.h"
 #include "Asset/AssetManager.h"
 #include "Math/Math.h"
 #include "Windows/DX/Sprite.h"
 
 MonsterBase::MonsterBase(const std::wstring& kName) :
    ServerObject(kName),
     state_(State::kIdle)
 {
 }
 
 void MonsterBase::BeginPlay()
 {
     ServerObject::BeginPlay();
 
     Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Box.png");
     if (sprite)
     {
         renderer_->SetSprite(sprite, L"Box_0");
         renderer_->SetColor(Math::Color::Red);
     }
 }

void MonsterBase::ReceivePacket(Net::IPacket* packet)
 {
     ServerObject::ReceivePacket(packet);

     
 }

 void MonsterBase::PhysicsTick(float delta_time)
 {
     ServerObject::PhysicsTick(delta_time);
     
 }
 
 RTTR_REGISTRATION
 {
     using namespace rttr;
 
     registration::class_<MonsterBase>("MonsterBase")
         .constructor<const std::wstring&>()
         (
             policy::ctor::as_std_shared_ptr
         );
 }