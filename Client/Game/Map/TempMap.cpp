#include "pch.h"
#include "TempMap.h"

#include "Logger.h"
#include "Input/Keyboard.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"

using WidgetPtr = std::shared_ptr<UI::Widget>;

WidgetPtr w1 = nullptr;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();

    w1 = UI::Widget::Create(L"W1");
    WidgetPtr w2 = UI::Widget::Create(L"W2");
    w2->AttachToWidget(w1.get());
    
    WidgetPtr w3 = UI::Widget::Create(L"W3");
    w3->AttachToWidget(w1.get());
    
    WidgetPtr w4 = UI::Widget::Create(L"W4");
    w4->AttachToWidget(w1.get());
    
}

void TempMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

    static bool is_open = false;
    
    Keyboard* keyboard = Keyboard::Get();
    if (keyboard->GetKeyDown('I'))
    {
        is_open = !is_open;
        if (is_open) UI::Manager::Get()->AddWidget(w1);
        else UI::Manager::Get()->RemoveWidget(w1);
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
