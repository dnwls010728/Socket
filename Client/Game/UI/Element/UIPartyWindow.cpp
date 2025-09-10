#include "pch.h"
#include "UIPartyWindow.h"

#include "UIPartyMemberSlot.h"
#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UIText.h"
#include "UI/Element/UIEditableText.h"
#include "Windows/DX/UISprite.h"
#include "Windows/DX/Renderer.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UIPopup.h"
#include <CustomPacket.h>

UIPartyWindow::UIPartyWindow(const std::wstring& name):
    UIContainer(name),
    host_id_(0),
    close_button_(nullptr),
    background_(nullptr),
    create_button_(nullptr),
    leave_button_(nullptr),
    empty_text_(nullptr),
    party_name_text_(nullptr)

{
    SetSize({500.f, 280.f});
}

void UIPartyWindow::Init()
{
    UISprite* panel_sprite  = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    UISprite* close_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\CloseButton.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetSize(GetSize());
    background_->SetIgnoreRayCast(true);

    close_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"CloseButton");
    close_button_->SetRelativePosition({GetSize().x - 30.f, 10.f});
    close_button_->SetSize({20.f, 20.f});
    close_button_->SetText(L"");
    close_button_->SetTextColor(Math::Color::White);
    close_button_->SetSprite(UIButton::State::kNormal,   close_sprite, L"CloseButton_0");
    close_button_->SetSprite(UIButton::State::kHover,    close_sprite, L"CloseButton_0");
    close_button_->SetSprite(UIButton::State::kPressed,  close_sprite, L"CloseButton_0");
    close_button_->SetSprite(UIButton::State::kDisabled, close_sprite, L"CloseButton_0");
    close_button_->OnClick(this, &UIPartyWindow::OnClose);

    empty_text_ = AddChild<UIText>(UIText::StaticClass(), L"EmptyText");
    empty_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    empty_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    empty_text_->SetColor(Math::Color::White);
    empty_text_->SetText(L"현재 파티에 가입되어 있지 않습니다.");

    create_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"CreateButton");
    create_button_->SetSize({120.f, 30.f});
    create_button_->SetText(L"파티 생성");
    create_button_->SetSprite(UIButton::State::kNormal,   button_sprite, L"ButtonSheet_0");
    create_button_->SetSprite(UIButton::State::kHover,    button_sprite, L"ButtonSheet_1");
    create_button_->SetSprite(UIButton::State::kPressed,  button_sprite, L"ButtonSheet_2");
    create_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    create_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    create_button_->SetTextColor(Math::Color::White);
    create_button_->OnClick(this, &UIPartyWindow::OnCreateParty);

    leave_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"LeaveButton");
    leave_button_->SetSize({120.f, 30.f});
    leave_button_->SetText(L"파티 탈퇴");
    leave_button_->SetSprite(UIButton::State::kNormal,   button_sprite, L"ButtonSheet_0");
    leave_button_->SetSprite(UIButton::State::kHover,    button_sprite, L"ButtonSheet_1");
    leave_button_->SetSprite(UIButton::State::kPressed,  button_sprite, L"ButtonSheet_2");
    leave_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    leave_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    leave_button_->SetTextColor(Math::Color::White);
    leave_button_->OnClick(this, &UIPartyWindow::OnLeaveParty);

    party_name_text_ = AddChild<UIText>(UIText::StaticClass(), L"PartyName");
    party_name_text_->SetColor(Math::Color::White);
    party_name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    party_name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    party_name_text_->SetText(L"");


    EnsureSlots(kInitialSlots);
    RefreshLayout();
    UIContainer::Init();
}

void UIPartyWindow::EnsureSlots(size_t required)
{
    size_t current = slots_.size();
    if (current >= required)
        return;
    
    size_t need = required - current;
    size_t add  = ((need + kBatchSlots - 1) / kBatchSlots) * kBatchSlots;
    for (size_t i = 0; i < add; ++i)
    {
        auto* slot = AddChild<UIPartyMemberSlot>(UIPartyMemberSlot::StaticClass(), L"Slot" + std::to_wstring(current + i));
        slot->SetSize({kSlotWidth, kSlotHeight});
        slot->SetEmpty(true);
        slots_.push_back(slot);
    }
}

void UIPartyWindow::AddOrUpdateMember(const PartyMemberInfo& info)
{
    members_[info.character_id] = info;
    size_t need = std::max(static_cast<size_t>(kInitialSlots), ((members_.size() + kBatchSlots - 1) / kBatchSlots) * kBatchSlots);
    EnsureSlots(need);
    RefreshSlots();
    RefreshLayout();
}

void UIPartyWindow::RemoveMember(uint32_t id)
{
    auto it = members_.find(id);
    if (it != members_.end())
        members_.erase(it);
    
    RefreshSlots();
    RefreshLayout();
}

void UIPartyWindow::Clear()
{
    members_.clear();
    for (auto* slot : slots_)
        slot->SetEmpty(true);
    RefreshLayout();
}

void UIPartyWindow::SetHost(uint32_t id)
{
    host_id_ = id;
    RefreshMembers();
}

void UIPartyWindow::RefreshMembers()
{
    RefreshSlots();
    RefreshLayout();
}

void UIPartyWindow::RefreshSlots()
{
    size_t idx = 0;
    for (auto& member_pair : members_)
    {
        if (idx >= slots_.size()) break;
        bool is_self       = member_pair.first == PlayerSubsystem::Get()->GetCharacterID();
        bool is_party_host = PlayerSubsystem::Get()->GetCharacterID() == host_id_;
        slots_[idx]->Update(member_pair.second, member_pair.first == host_id_, is_self, is_party_host);
        ++idx;
    }
    for (; idx < slots_.size(); ++idx) slots_[idx]->SetEmpty(true);
}

void UIPartyWindow::RefreshLayout()
{
    auto place_close = [this]()
    {
        if (close_button_) close_button_->SetRelativePosition({GetSize().x - 30.f, 10.f});
    };

    if (members_.empty())
    {
        float tw = 0.f, th = 0.f;
        if (empty_text_) Renderer::Get()->GetTextSize(empty_text_->GetText(), empty_text_->GetFontName(), empty_text_->GetFontSize(), tw, th);
        const float btn_w = create_button_ ? create_button_->GetSize().x : 0.f;
        const float btn_h = create_button_ ? create_button_->GetSize().y : 0.f;

        float width = std::max({260.f, tw + kPad * 2.f, btn_w + kPad * 2.f});
        float height = kTopBarHeight + kPad + th + 6.f + btn_h + kBottomPad;

        SetSize({width, height});
        if (background_)
            background_->SetSize(GetSize());
        place_close();

        if (empty_text_)
        {
            empty_text_->SetActive(true);
            empty_text_->SetSize({GetSize().x - kPad * 2.f, th});
            empty_text_->SetRelativePosition({kPad, kTopBarHeight + kPad});
        }
        if (create_button_)
        {
            create_button_->SetActive(true);
            create_button_->SetRelativePosition({GetSize().x - kPad - btn_w, GetSize().y - kBottomPad - btn_h});
        }
        if (leave_button_)
            leave_button_->SetActive(false);
        for (auto* slot : slots_)
            slot->SetActive(false);
        return;
    }

    if (empty_text_)   empty_text_->SetActive(false);
    if (create_button_) create_button_->SetActive(false);
    if (leave_button_)  leave_button_->SetActive(true);
    for (auto* slot : slots_)
        slot->SetActive(true);

    const int rows = static_cast<int>((slots_.size() + kCols - 1) / kCols);
    const float grid_w = kCols * kSlotWidth + (kCols - 1) * kGapX;
    const float grid_h = rows * kSlotHeight + (rows - 1) * kGapY;

    const float btn_w = leave_button_ ? leave_button_->GetSize().x : 0.f;
    const float btn_h = leave_button_ ? leave_button_->GetSize().y : 0.f;

    const float new_w = std::max(grid_w + kPad * 2.f, btn_w + kPad * 2.f);
    const float new_h = kTopBarHeight + kPad + grid_h + 6.f + btn_h + kBottomPad;

    SetSize({new_w, new_h});
    if (background_)
        background_->SetSize(GetSize());
    place_close();

    if (leave_button_)
        leave_button_->SetRelativePosition({GetSize().x - kPad - btn_w, GetSize().y - kBottomPad - btn_h});

    int i = 0;
    for (auto* slot : slots_)
    {
        int col = i % kCols;
        int row = i / kCols;
        float x = kPad + col * (kSlotWidth + kGapX);
        float y = kTopBarHeight + kPad + row * (kSlotHeight + kGapY);
        slot->SetRelativePosition({x, y});
        slot->SetSize({kSlotWidth, kSlotHeight});
        ++i;
    }

    if (party_name_text_)
        party_name_text_->SetRelativePosition({GetSize().x + 10, GetSize().y + 10});
}

bool UIPartyWindow::OnDragBegin(const Math::Vector2&) { return true; }

bool UIPartyWindow::OnDrag(const Math::Vector2&, const Math::Vector2& delta)
{
    Math::Vector2 p = GetRelativePosition() + delta;
    SetRelativePosition(p);
    return true;
}

bool UIPartyWindow::OnDragEnd(const Math::Vector2&) { return true; }

void UIPartyWindow::OnClose()
{
    SetActive(false);
}

void UIPartyWindow::OnCreateParty()
{
    UIPopup::PopupParam param;
    param.caption = L"파티 이름을 입력하세요.";
    param.option  = UIPopup::PopupOption::OK | UIPopup::PopupOption::Cancel | UIPopup::PopupOption::Edit;
    param.callback = [this](const std::wstring& text, UIPopup::PopupOption option)
    {
        if (option == UIPopup::PopupOption::OK)
        {
            PartyCreateRequest request;
            request.party_name = text;
            SessionSubsystem::Get()->SendPacket(request);
        }
        return true;
    };
    UIPopup::ShowPopup(param);
}

void UIPartyWindow::OnLeaveParty()
{
    UIPopup::PopupParam param;
    param.caption = L"파티에서 탈퇴하시겠습니까?";
    param.option  = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
    param.callback = [&](const std::wstring&, UIPopup::PopupOption opt)->bool {
        if (opt == UIPopup::PopupOption::Yes)
        {
            PartyLeavePacket req;
            SessionSubsystem::Get()->SendPacket(req);
        }
        return true;
    };
    UIPopup::ShowPopup(param);
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UIPartyWindow>("UIPartyWindow")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
