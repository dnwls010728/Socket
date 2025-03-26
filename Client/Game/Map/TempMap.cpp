#include "pch.h"
#include "TempMap.h"

#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/Widget/TestWidget.h"

using WidgetPtr = std::shared_ptr<UI::Widget>;

WidgetPtr w1 = nullptr;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();

    w1 = UI::TestWidget::Create(L"W1");
    w1->SetPosition({0, 0});
    w1->SetSize({300, 300});
    
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
            UI::Manager::Get()->AddWidget(w1);
        }
        else
        {
            UI::Manager::Get()->RemoveWidget(w1);
        }
    }

    static int size[2] = {300, 300};
    if (ImGui::SliderInt2("Size", size, 0, 1000))
    {
        w1->SetSize({static_cast<float>(size[0]), static_cast<float>(size[1])});
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
