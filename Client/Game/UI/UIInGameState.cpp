#include "pch.h"
#include "UIInGameState.h"

#include "PostProcessingSettings.h"
#include "UI/Element/UIContextMenu.h"
#include "Element/UIChatBar.h"
#include "Element/UIMenu.h"
#include "Element/UIMiniMap.h"
#include "Element/UIStatusBar.h"
#include "Element/UIPartyPanel.h"
#include "Element/UIPartyWindow.h"
#include "Element/Buff/UIBuffList.h"
#include "Element/Inventory/UIInventory.h"
#include "Element/Inventory/UIItemTooltip.h"
#include "Element/QuickSlot/UIQuickBar.h"
#include "Element/Skill/UISkillWindow.h"
#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "Subsystems/PartySubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/InputActions/InputActions.h"

UIInGameState::UIInGameState() :
    context_menu_(nullptr),
    party_panel_(nullptr),
    party_window_(nullptr),
    show_post_process_(false)
{
    AddElement<UIMiniMap>(UIMiniMap::StaticClass(), L"MiniMap");
    
    char_bar_ = AddElement<UIChatBar>(UIChatBar::StaticClass(), L"ChatBar");
    char_bar_->SetAbsolutePosition({0.f, 684.f});
    
    inventory_ = AddElement<UIInventory>(UIInventory::StaticClass(), L"Inventory");
    inventory_->SetActive(false);

    equipment_ = AddElement<UIEquipment>(UIEquipment::StaticClass(), L"Equipment");
    equipment_->SetActive(false);

    skill_window_ = AddElement<UISkillWindow>(UISkillWindow::StaticClass(), L"SkillWindow");
    skill_window_->SetActive(false);
    skill_window_->SetAbsolutePosition({ 350.f, 120.f });
    
    AddElement<UIStatusBar>(UIStatusBar::StaticClass(), L"StatusBar");
    
    party_panel_ = AddElement<UIPartyPanel>(UIPartyPanel::StaticClass(), L"PartyPanel");
    party_panel_->SetAbsolutePosition({EngineSettings::Get()->GetScreenWidth() - party_panel_->GetSize().x - 25, 25});
    party_panel_->SetActive(false);

    party_window_ = AddElement<UIPartyWindow>(UIPartyWindow::StaticClass(), L"PartyWindow");
    party_window_->SetAbsolutePosition({100.f, 100.f});
    party_window_->SetActive(false);

    item_tooltip_ = AddElement<UIItemTooltip>(UIItemTooltip::StaticClass(), L"ItemTooltip");
    item_tooltip_->SetAbsolutePosition({ 100.f, 100.f });
    item_tooltip_->SetSize({ 322.f, 122.f });
    item_tooltip_->SetActive(false);
    item_tooltip_->SetIgnoreRayCast(true);

    menu_ = AddElement<UIMenu>(UIMenu::StaticClass(), L"Menu");
    menu_->SetActive(false);

    context_menu_ = AddElement<UIContextMenu>(UIContextMenu::StaticClass(), L"ContextMenu");
    context_menu_->SetActive(false);

    AddElement<UIBuffList>(UIBuffList::StaticClass(), L"BuffList");

    auto* quick_bar = AddElement<UIQuickBar>(UIQuickBar::StaticClass(), L"QuickBar");

    const Math::Vector2& size = quick_bar->GetSize();
    quick_bar->SetAbsolutePosition({ 1366.f - size.x - 10.f, 768.f - size.y - 19.f });
}

void UIInGameState::Tick(float delta_time)
{
    UIState::Tick(delta_time);

    if (Keyboard::Get()->GetKeyDown(Scancode::kKeyR))
        show_post_process_ = !show_post_process_;

    if (show_post_process_)
    {
        if (!ImGui::Begin("Post Process", &show_post_process_))
        {
            ImGui::End();
            return;
        }

        PostProcessingSettings* settings = PostProcessingSettings::Get();

        static float blur_radius = settings->GetBlurRadius();
        static float vignette_strength = settings->GetVignetteStrength();
        static float gamma = settings->GetGamma();
        static float grayscale = settings->GetGrayscale();

        if (ImGui::SliderFloat("Blur Radius", &blur_radius, 0.f, 20.f))
            settings->SetBlurRadius(blur_radius);

        if (ImGui::SliderFloat("Vignette Strength", &vignette_strength, 0.f, 1.f))
            settings->SetVignetteStrength(vignette_strength);

        if (ImGui::SliderFloat("Gamma", &gamma, 0.1f, 5.f))
            settings->SetGamma(gamma);

        if (ImGui::SliderFloat("Grayscale", &grayscale, 0.f, 1.f))
            settings->SetGrayscale(grayscale);

        ImGui::End();
    }
}

void UIInGameState::Init()
{
    UIState::Init();
    PartySubsystem::Get()->RedrawUI();
}

bool UIInGameState::OnKey(uint32_t scancode, bool is_pressed)
{
    bool is_handled = UIState::OnKey(scancode, is_pressed);
    if (is_handled || IsEditingText()) return true;
    
    InputActions* actions = InputActions::Get();
    InputActions::Mapping mapping = actions->GetMapping(scancode);

    PlayerSubsystem* player = PlayerSubsystem::Get();
    
    if (is_pressed)
    {
        if (scancode == static_cast<uint32_t>(Scancode::kKeyEnter))
        {
            char_bar_->FocusInput();
            is_handled = true;
        }
        else
        {
            switch (mapping.type)
            {
            case static_cast<uint8_t>(KeyType::kItem):
                player->UseItem(mapping.action);
                is_handled = true;
                break;
            case static_cast<uint8_t>(KeyType::kSkill):
                player->UseSkill(mapping.action);
                is_handled = true;
                break;
            case static_cast<uint8_t>(KeyType::kMenu):
                switch (mapping.action)
                {
            case static_cast<uint8_t>(KeyAction::kItems):
                    inventory_->SetActive(!inventory_->IsActive());
                    is_handled = true;
                    break;
            case static_cast<uint8_t>(KeyAction::kEquipment):
                    equipment_->SetActive(!equipment_->IsActive());
                    is_handled = true;
                    break;
            case static_cast<uint8_t>(KeyAction::kSkills):
                    skill_window_->SetActive(!skill_window_->IsActive());
                    is_handled = true;
                    break;
            case static_cast<uint8_t>(KeyAction::kParty):
                    party_window_->SetActive(!party_window_->IsActive());
                    is_handled = true;
                    break;
            case static_cast<uint8_t>(KeyAction::kMainMenu):
                    menu_->SetActive(!menu_->IsActive());
                    is_handled = true;
                    break;
                }
                break;
            }
        }
    }
    
    return is_handled;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInGameState>("UIInGameState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
