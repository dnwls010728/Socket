#include "pch.h"
#include "TempMap.h"

#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/Widget/Button.h"

UI_ButtonPtr button = nullptr;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();

    button = UI::Button::Create(L"Button");
    button->SetPosition({100.f, 100.f});
    button->SetSize({100.f, 50.f});
    button->SetPivot({.5f, .5f});
    button->AddEvent([&]()
    {
        Logger::Print(L"Button clicked");
    });
    
}

void TempMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

    static bool is_open = false;
    
    Keyboard* keyboard = Keyboard::Get();
    if (keyboard->GetKeyDown('I'))
    {
        is_open = !is_open;
        if (is_open)
        {
            UI::Manager::Get()->AddToViewport(button);
        }
        else
        {
            UI::Manager::Get()->RemoveFromViewport(button);
        }
    }

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
