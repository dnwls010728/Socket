#include "pch.h"
#include "TempMap.h"

#include "Logger.h"

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

    Logger::Print(L"Level Tick!");
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<TempMap>("TempMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
