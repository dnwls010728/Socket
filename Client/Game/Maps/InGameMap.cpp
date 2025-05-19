#include "pch.h"
#include "InGameMap.h"

#include "UI/UI.h"
#include "UI/UIInGameState.h"

InGameMap::InGameMap(const std::wstring& kName) :
    Level(kName)
{
}

void InGameMap::Load()
{
    Level::Load();

    UI::Get()->ChangeState(UIInGameState::StaticClass());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<InGameMap>("InGameMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
