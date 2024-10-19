#pragma once
#include <map>
#include <string>

#include "Math/MathTypes.h"

using BYTE = unsigned char;


// 현재 헤더파일에 있는 타입들은 프로젝트에 종속적인 타입들입니다.
enum class ActorTag : MathTypes::uint64
{
    kNone,
};

enum ActorLayer : MathTypes::uint16
{
    kDefault = (0x01<<0),
    kGround = (0x01<<1),
    kPlayer = (0x01<<2),
};

class ProjectSettings
{
public:
    inline static const std::wstring kWindowTitle = L"ProjectM"; // 윈도우 타이틀

    inline static constexpr MathTypes::uint32 kScreenWidth = 640; // 화면 너비
    inline static constexpr MathTypes::uint32 kScreenHeight = 480; // 화면 높이

    inline static constexpr MathTypes::uint32 kCanvasReferenceWidth = 640;
    inline static constexpr MathTypes::uint32 kCanvasReferenceHeight = 480;
    inline static constexpr float kMatchMode = 0.f;
    
    inline static constexpr bool kShowFrameRate = true; // 프레임레이트 표시 여부
    inline static constexpr bool kUseVSync = false; // 수직동기화 사용 여부

    inline static constexpr float kFixedTimeStep = .02f; // 고정 프레임 간격

    inline static const std::map<MathTypes::uint16, MathTypes::uint16> kLayerCollisionMatrix = { // 레이어 충돌 매트릭스
        {kDefault, kDefault | kGround | kPlayer},
        {kGround, kGround | kDefault | kPlayer},
        {kPlayer, kPlayer | kGround | kDefault},
    };
    
};
