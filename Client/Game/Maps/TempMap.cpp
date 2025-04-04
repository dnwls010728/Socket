#include "pch.h"
#include "TempMap.h"

#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
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

    room_list = UI::ListBox::Create(L"RoomList");
    room_list->SetPosition({320.f, 240.f});
    room_list->SetSize({300.f, 150.f});
    
    room_list->OnSelect([&](uint64_t user_data) {
        text_box->SetText(L"ROOM " + std::to_wstring(user_data));
    });

    room_list->OnDoubleClick([&](uint64_t user_data) {
        text_box->SetText(L"DOUBLE CLICK ROOM " + std::to_wstring(user_data));
    });
    
    UI::Manager::Get()->AddToViewport(room_list);

    text_box = UI::TextBox::Create(L"TextBox");
    text_box->SetPosition({320.f, 400.f});
    text_box->SetSize({300.f, 50.f});

    UI::Manager::Get()->AddToViewport(text_box);

    std::shared_ptr<UI::Button> add_button = UI::Button::Create(L"AddRoom");
    add_button->SetPosition({200.f, 460.f});
    add_button->SetSize({100.f, 50.f});
    add_button->SetText(L"Add Room");
    add_button->OnClick([&]()
    {
        room_list->AddItem(L"Room " + std::to_wstring(room_list->GetItems().size()), room_list->GetItems().size());
    });

    UI::Manager::Get()->AddToViewport(add_button);

    std::shared_ptr<UI::Button> remove_button = UI::Button::Create(L"RemoveRoom");
    remove_button->SetPosition({440.f, 460.f});
    remove_button->SetSize({100.f, 50.f});
    remove_button->SetText(L"Remove Room");
    remove_button->OnClick([&]()
    {
        room_list->RemoveItem(room_list->GetSelectedIndex());
    });

    UI::Manager::Get()->AddToViewport(remove_button);

    std::shared_ptr<UI::Button> clear_button = UI::Button::Create(L"ClearRoom");
    clear_button->SetPosition({320.f, 460.f});
    clear_button->SetSize({100.f, 50.f});
    clear_button->SetText(L"Clear Room");
    clear_button->OnClick([&]()
    {
        room_list->ClearItems();
    });

    UI::Manager::Get()->AddToViewport(clear_button);

    std::shared_ptr<UI::EditableTextBox> editable_text_box = UI::EditableTextBox::Create(L"EditableTextBox");
    editable_text_box->SetPosition({100.f, 25.f});
    editable_text_box->SetSize({200.f, 50.f});
    editable_text_box->OnValueChanged([&](const std::wstring& kValue)
    {
        Logger::Print(L"EditableTextBox Value Changed: %s", kValue.c_str());
    });

    editable_text_box->OnReturn([&](const std::wstring& kValue)
    {
        Logger::Print(L"EditableTextBox Return: %s", kValue.c_str());
    });

    UI::Manager::Get()->AddToViewport(editable_text_box);
    
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
