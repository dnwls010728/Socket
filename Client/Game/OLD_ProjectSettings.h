#pragma once
#include <map>
#include <string>

#include "Singleton.h"
#include "Misc/EnumClassFlags.h"
#include "Misc/Type.h"

// 현재 헤더파일에 있는 타입들은 프로젝트에 종속적인 타입들입니다.
// enum class OLD_ActorTag : Type::uint64
// {
//     kNone,
// };
//
// enum class OLD_ActorLayer : Type::uint16
// {
//     kDefault = (0x01<<0),
//     kGround = (0x01<<1),
//     kPlayer = (0x01<<2),
//     kMob = (0x01<<3),
//     kBullet = (0x01<<4),
//     kBlock = (0x01<<5),
// };
//
// ENUM_CLASS_FLAGS(OLD_ActorLayer)

class OLD_ProjectSettings
{
public:
    // inline static const std::wstring kWindowTitle = L"ProjectM"; // 윈도우 타이틀
    //
    // inline static constexpr Type::uint32 kScreenWidth = 1280; // 화면 너비
    // inline static constexpr Type::uint32 kScreenHeight = 720; // 화면 높이

    inline static constexpr Type::uint32 kCanvasReferenceWidth = 640;
    inline static constexpr Type::uint32 kCanvasReferenceHeight = 360;
    inline static constexpr float kMatchMode = 0.f;
    
    // inline static constexpr bool kUseVSync = false; // 수직동기화 사용 여부
    //
    // inline static constexpr float kFixedTimeStep = .02f; // 고정 프레임 간격
    //
    // inline static const std::map<OLD_ActorLayer, OLD_ActorLayer> kLayerCollisionMatrix = { // 레이어 충돌 매트릭스
    //     {OLD_ActorLayer::kDefault, OLD_ActorLayer::kDefault | OLD_ActorLayer::kGround | OLD_ActorLayer::kPlayer | OLD_ActorLayer::kMob | OLD_ActorLayer::kBullet | OLD_ActorLayer::kBlock},
    //     {OLD_ActorLayer::kGround, OLD_ActorLayer::kGround | OLD_ActorLayer::kDefault | OLD_ActorLayer::kPlayer | OLD_ActorLayer::kMob | OLD_ActorLayer::kBullet | OLD_ActorLayer::kBlock},
    //     {OLD_ActorLayer::kPlayer, OLD_ActorLayer::kGround | OLD_ActorLayer::kDefault | OLD_ActorLayer::kBlock},
    //     {OLD_ActorLayer::kMob, OLD_ActorLayer::kBullet | OLD_ActorLayer::kGround | OLD_ActorLayer::kDefault | OLD_ActorLayer::kBlock},
    //     {OLD_ActorLayer::kBullet, OLD_ActorLayer::kMob | OLD_ActorLayer::kBlock},
    //     {OLD_ActorLayer::kBlock, OLD_ActorLayer::kDefault | OLD_ActorLayer::kGround | OLD_ActorLayer::kPlayer | OLD_ActorLayer::kMob | OLD_ActorLayer::kBullet}
    // };
    
};
