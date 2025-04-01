#include "pch.h"
#include "TempMap.h"

#include "Editor/Editor.h"
#include "UI/UIManager.h"
#include "UI/Widget/ListBox.h"
#include "UI/Widget/TextBox.h"

std::shared_ptr<UI::TextBox> text_box = nullptr;

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();
    
    // AddActor<PlayerCharacter>(L"Player");
    // AddActor<DefaultTilemap>(L"Tilemap");
    // AddActor<Editor>(L"Editor");

    std::shared_ptr<UI::ListBox> room_list = std::make_shared<UI::ListBox>(L"RoomList");
    room_list->SetPosition({320.f, 240.f});
    room_list->SetSize({300.f, 150.f});

    for (int i = 0; i < 20; ++i)
    {
        room_list->AddItem(L"ROOM " + std::to_wstring(i), i);
    }
    
    room_list->OnSelect([&](uint64_t user_data) {
        text_box->SetText(L"ROOM " + std::to_wstring(user_data));
    });
    
    UI::Manager::Get()->AddToViewport(room_list);

    text_box = std::make_shared<UI::TextBox>(L"TextBox");
    text_box->SetPosition({320.f, 400.f});
    text_box->SetSize({300.f, 50.f});

    UI::Manager::Get()->AddToViewport(text_box);
    
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
