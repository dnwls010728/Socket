#include "pch.h"
#include "EditableTextBox.h"

#include <numeric>

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::EditableTextBox::EditableTextBox(const std::wstring& kName) :
    Widget(kName),
    font_name_(L"NanumBarunGothic"),
    font_size_(12.f),
    text_(L""),
    placeholder_(L""),
    elapsed_time_(0.f),
    total_advance_(0.f),
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

void UI::EditableTextBox::SetText(const std::wstring& kText)
{
    text_ = kText;
    cursor_position_ = text_.size();
    total_advance_ = GetAdvances(text_, advances_);
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

    const Math::Rect rect = GetRect();

    renderer->DrawSolidBox(window, rect, GetPivotPosition(), {0, 0, 0, 100});
    renderer->DrawBox(window, rect, GetPivotPosition(), Math::Color::White);
    
    float cursor_advance = 0.f;
    float text_offset = 0.f;
    
    if (cursor_position_ - 1 < advances_.size())
        cursor_advance = std::accumulate(advances_.begin(), advances_.begin() + cursor_position_, 0.f);

    if (rect.width < cursor_advance) text_offset = cursor_advance - rect.width;

    renderer->BeginLayer(rect);

    Math::Rect text_rect = GetRect(
        {rect.x - text_offset, rect.y},
        {total_advance_ + 1.f, rect.height},
        {0.f, 1.f}
    );

    Math::Rect placeholder_rect = GetRect(
        {rect.x, rect.y},
        {rect.width, rect.height},
        {0.f, 1.f}
    );

    renderer->DrawString(window, text_, text_rect,GetPivotPosition(text_rect, {0.f, 1.f}), Math::Color::White, 0.f, font_name_, font_size_, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    
    if (text_.empty())
    {
        renderer->DrawString(window, placeholder_, placeholder_rect,GetPivotPosition(placeholder_rect, {0.f, 1.f}), Math::Color::Gray, 0.f, font_name_, font_size_, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }
    
    renderer->EndLayer();

    if (cursor_visible_)
    {
        float font_size = renderer->GetTextFormat(font_name_, font_size_)->GetFontSize();
        float padding = (rect.height - font_size) * .5f;

        Math::Vector2 start = {rect.x + cursor_advance - text_offset, rect.y + padding};
        Math::Vector2 end = {rect.x + cursor_advance - text_offset, rect.y + rect.height - padding};

        renderer->DrawLine(window, start, end, Math::Color::White, 2.f);
    }
}

bool UI::EditableTextBox::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed,
    double timestamp)
{
    Widget::OnMouseButton(kPosition, button, is_pressed, timestamp);
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
                
                total_advance_ = GetAdvances(text_, advances_);

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
                
                total_advance_ = GetAdvances(text_, advances_);

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
    
    total_advance_ = GetAdvances(text_, advances_);

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
    renderer->GetTextAdvances(kString, font_name_, font_size_, advances);

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
