#include "pch.h"
#include "TempMap.h"

#include "Editor/Editor.h"
#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/ListBox.h"
#include "UI/Widget/TextBox.h"

std::shared_ptr<UI::ListBox> room_list = nullptr;
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

    room_list = UI::ListBox::Create(L"RoomList");
    room_list->SetPosition({320.f, 240.f});
    room_list->SetSize({300.f, 150.f});
    
    room_list->OnSelect([&](uint64_t user_data) {
        text_box->SetText(L"ROOM " + std::to_wstring(user_data));
    });
    
    UI::Manager::Get()->AddToViewport(room_list);

    text_box = UI::TextBox::Create(L"TextBox");
    text_box->SetPosition({320.f, 400.f});
    text_box->SetSize({300.f, 50.f});

    UI::Manager::Get()->AddToViewport(text_box);

    std::shared_ptr<UI::Button> add_button = UI::Button::Create(L"AddRoom");
    add_button->SetPosition({220.f, 460.f});
    add_button->SetSize({100.f, 50.f});
    add_button->SetText(L"Add Room");
    add_button->OnClick([&]()
    {
        room_list->AddItem(L"Room " + std::to_wstring(room_list->GetItems().size()), room_list->GetItems().size());
    });

    UI::Manager::Get()->AddToViewport(add_button);

    std::shared_ptr<UI::Button> remove_button = UI::Button::Create(L"RemoveRoom");
    remove_button->SetPosition({420.f, 460.f});
    remove_button->SetSize({100.f, 50.f});
    remove_button->SetText(L"Remove Room");
    remove_button->OnClick([&]()
    {
        room_list->RemoveItem(room_list->GetSelectedIndex());
    });

    UI::Manager::Get()->AddToViewport(remove_button);
    
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
