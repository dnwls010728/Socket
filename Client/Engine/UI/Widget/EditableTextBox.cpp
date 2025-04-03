#include "pch.h"
#include "EditableTextBox.h"

#include <numeric>

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

// 테스트
float start_position = 0.f;
float end_position = 0.f;

std::wstring selected_text = L"";

UI::EditableTextBox::EditableTextBox(const std::wstring& kName) :
    Widget(kName),
    text_(L""),
    elapsed_time_(0.f),
    cursor_visible_(false),
    cursor_position_(0),
    value_changed_event_([&](const std::wstring& kValue) {}),
    return_event_([&](const std::wstring& kValue) {}),
    advances_()
{
}

void UI::EditableTextBox::OnValueChanged(void(* func)(const std::wstring&))
{
    value_changed_event_ = func;
}

void UI::EditableTextBox::OnReturn(void(* func)(const std::wstring&))
{
    return_event_ = func;
}

std::shared_ptr<UI::EditableTextBox> UI::EditableTextBox::Create(const std::wstring& kName)
{
    return std::make_shared<EditableTextBox>(kName);
}

void UI::EditableTextBox::Tick(float delta_time)
{
    Widget::Tick(delta_time);

    if (IsFocused())
    {
        elapsed_time_ += delta_time;
        if (elapsed_time_ > .5f)
        {
            cursor_visible_ = !cursor_visible_;
            elapsed_time_ = 0.f;
        }
    }
}

void UI::EditableTextBox::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    renderer->DrawBox(window, GetRect(), GetPivotPosition(), Math::Color::Black, 0.f);

    renderer->BeginLayer(GetRect());
    renderer->DrawString(window, text_, GetRect(), GetPivotPosition(), Math::Color::White, 0.f, L"Nanum18", DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    renderer->EndLayer();
}

bool UI::EditableTextBox::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed,
    double timestamp)
{
    Widget::OnMouseButton(kPosition, button, is_pressed, timestamp);
    return true;
}

bool UI::EditableTextBox::OnDragBegin(const Math::Vector2& kPosition)
{
    if (text_.empty()) return false;
    start_position = kPosition.x;
    
    return true;
}

bool UI::EditableTextBox::OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    if (text_.empty()) return false;
    
    return true;
}

bool UI::EditableTextBox::OnDragEnd(const Math::Vector2& kPosition)
{
    if (text_.empty()) return false;
    end_position = kPosition.x;
    
    return true;
}

bool UI::EditableTextBox::OnKey(Type::uint16 key_code, bool is_pressed)
{
    if (is_pressed)
    {
        if (key_code == VK_LEFT)
        {
            if (cursor_position_ > 0)
            {
                cursor_position_--;

                elapsed_time_ = 0.f;
                cursor_visible_ = true;
                return true;
            }
        }
        else if (key_code == VK_RIGHT)
        {
            if (cursor_position_ < text_.size())
            {
                cursor_position_++;

                elapsed_time_ = 0.f;
                cursor_visible_ = true;
                return true;
            }
        }
        else if (key_code == VK_RETURN)
        {
            return_event_(text_);
            
            elapsed_time_ = 0.f;
            cursor_visible_ = false;
            return true;
        }
        else if (key_code == VK_BACK)
        {
            if (!text_.empty() && cursor_position_ > 0)
            {
                text_.erase(cursor_position_-- - 1, 1);
                value_changed_event_(text_);
                
                float total_advance = GetAdvances(text_, advances_);
                Logger::Print(L"Total Advance: %f", total_advance);

                elapsed_time_ = 0.f;
                cursor_visible_ = true;
                return true;
            }
        }
        else if (key_code == VK_HOME)
        {
            cursor_position_ = 0;

            elapsed_time_ = 0.f;
            cursor_visible_ = true;
            return true;
        }
        else if (key_code == VK_END)
        {
            cursor_position_ = text_.size();

            elapsed_time_ = 0.f;
            cursor_visible_ = true;
            return true;
        }
        else if (key_code == VK_DELETE)
        {
            if (!text_.empty() && cursor_position_ < text_.size())
            {
                text_.erase(cursor_position_, 1);
                value_changed_event_(text_);
                
                float total_advance = GetAdvances(text_, advances_);
                Logger::Print(L"Total Advance: %f", total_advance);

                elapsed_time_ = 0.f;
                cursor_visible_ = true;
                return true;
            }
        }
    }
    
    return false;
}

bool UI::EditableTextBox::OnChar(wchar_t character)
{
    text_.insert(cursor_position_++, 1, character);
    value_changed_event_(text_);
    
    float total_advance = GetAdvances(text_, advances_);
    Logger::Print(L"Total Advance: %f", total_advance);

    // Space Bar
    if (character == 32)
    {
        elapsed_time_ = 0.f;
        cursor_visible_ = true;
    }
    
    return true;
}

void UI::EditableTextBox::OnFocus(bool is_focus)
{
    elapsed_time_ = 0.f;
    cursor_visible_ = is_focus;
    Widget::OnFocus(is_focus);
}

float UI::EditableTextBox::GetAdvances(const std::wstring& kString, std::vector<float>& advances)
{
    Renderer* renderer = Renderer::Get();
    renderer->GetTextAdvances(kString, L"Nanum18", advances);

    float total_advance = std::accumulate(advances.begin(), advances.end(), 0.f);
    return total_advance;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::EditableTextBox>("UI::EditableTextBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
