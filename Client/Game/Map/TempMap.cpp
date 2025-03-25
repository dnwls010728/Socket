#include "pch.h"
#include "TempMap.h"

#include "Logger.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"

using WidgetPtr = std::shared_ptr<UI::Widget>;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();

    WidgetPtr w1 = UI::Widget::Create(L"W1");
    WidgetPtr w2 = UI::Widget::Create(L"W2");
    w2->AttachToWidget(w1.get());
    
    WidgetPtr w3 = UI::Widget::Create(L"W3");
    w3->AttachToWidget(w1.get());
    
    WidgetPtr w4 = UI::Widget::Create(L"W4");
    w4->AttachToWidget(w1.get());

    UI::Manager::Get()->AddWidget(w1);
    
}

void TempMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

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
