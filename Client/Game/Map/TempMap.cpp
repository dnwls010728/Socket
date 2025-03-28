#include "pch.h"
#include "TempMap.h"

#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/CheckBox.h"
#include "UI/Widget/Slider.h"

UI_ButtonPtr b1 = nullptr;
UI_SliderPtr s1 = nullptr;
UI_CheckBoxPtr c1 = nullptr;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName),
    is_open_(false)
{
}

void TempMap::Load()
{
    Level::Load();

    b1 = UI::Button::Create(L"Button1");
    b1->SetPosition({100.f, 100.f});
    b1->SetSize({100.f, 50.f});
    b1->SetPivot({.5f, .5f});
    b1->AddClickEvent([&]()
    {
        Logger::Print(L"Button clicked");
    });

    s1 = UI::Slider::Create(L"Slider1");
    s1->SetPosition({100.f, 200.f});
    s1->SetSize({120.f, 16.f});
    s1->SetPivot({.5f, .5f});
    s1->AddValueChangedEvent([&](float value)
    {
        Logger::Print(L"%f", value);
    });

    c1 = UI::CheckBox::Create(L"CheckBox1");
    c1->SetPosition({300.f, 300.f});
    c1->SetSize({16.f, 16.f});
    c1->AddCheckEvent([&](bool value)
    {
        is_open_ = value;
    });
    
    UI::Manager::Get()->AddToViewport(s1);
    UI::Manager::Get()->AddToViewport(c1);
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
            UI::Manager::Get()->AddToViewport(s1);
        }
        else
        {
            UI::Manager::Get()->RemoveFromViewport(s1);
        }
    }

    if (is_open_)
    {
        ImGui::Begin("Settings");
        ImGui::Text("Hello, world!");
        ImGui::End();
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
