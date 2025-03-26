#include "pch.h"
#include "TempMap.h"

#include "Input/Keyboard.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/Widget/TestWidget.h"

using WidgetPtr = std::shared_ptr<UI::Widget>;

WidgetPtr w1 = nullptr;
WidgetPtr w2 = nullptr;
WidgetPtr w3 = nullptr;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();

    w1 = UI::TestWidget::Create(L"W1");
    w1->SetPosition({0, 0});
    w1->SetSize({100, 100});
    
    w2 = UI::TestWidget::Create(L"W2");
    w2->SetPosition({10, 10});
    w2->SetSize({100, 100});
    
    w3 = UI::TestWidget::Create(L"W3");
    w3->SetPosition({20, 20});
    w3->SetSize({100, 100});
    
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
            UI::Manager::Get()->AddWidget(w2);
            UI::Manager::Get()->AddWidget(w3);
        }
        else
        {
            UI::Manager::Get()->RemoveWidget(w1);
            UI::Manager::Get()->RemoveWidget(w2);
            UI::Manager::Get()->RemoveWidget(w3);
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
