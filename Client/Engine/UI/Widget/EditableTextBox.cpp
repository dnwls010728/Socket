#include "pch.h"
#include "EditableTextBox.h"

#include <numeric>

#include "Level/World.h"
#include "Math/Color.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"

EditableTextBox::EditableTextBox(const std::wstring& kName) :
    Widget(kName),
    text_(L""),
    cursor_position_(0),
    elapsed_time_(0.f),
    cursor_visible_(false),
    advances_()
{
    is_ray_cast_target_ = true;
}

void EditableTextBox::Tick(float delta_time)
{
    Widget::Tick(delta_time);
    if (!is_focused_) return;

    elapsed_time_ += delta_time;
    if (elapsed_time_ > .5f)
    {
        cursor_visible_ = !cursor_visible_;
        elapsed_time_ = 0.f;
    }
}

void EditableTextBox::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    Renderer* renderer = Renderer::Get();
    if (!renderer) return;
    
    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    renderer->DrawString(window, text_, rect_, pivot_position, Math::Color::White, angle_, L"Nanum18", DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    if (cursor_visible_)
    {
        float advance = 0.f;
        if (cursor_position_ - 1 < advances_.size())
        {
            advance = std::accumulate(advances_.begin(), advances_.begin() + cursor_position_, 0.f);
        }

        Math::Vector2 start = {rect_.x + advance, rect_.y};
        Math::Vector2 end = start + Math::Vector2(0.f, rect_.height);
        renderer->DrawLine(window, start, end, Math::Color::White, 1.f);
    }
    
    Widget::Render();
}

void EditableTextBox::OnFocusChanged(bool is_focused)
{
    Widget::OnFocusChanged(is_focused);

    if (!is_focused)
    {
        elapsed_time_ = 0.f;
        cursor_visible_ = false;
    }
}

void EditableTextBox::OnInputKey(Type::uint16 key_code, bool is_pressed)
{
    Widget::OnInputKey(key_code, is_pressed);

    if (is_pressed)
    {
        if (key_code == VK_LEFT)
        {
            if (cursor_position_ > 0)
            {
                cursor_position_--;

                elapsed_time_ = 0.f;
                cursor_visible_ = true;
            }
        }
        else if (key_code == VK_RIGHT)
        {
            if (cursor_position_ < text_.size())
            {
                cursor_position_++;
                
                elapsed_time_ = 0.f;
                cursor_visible_ = true;
            }
        }
        else if (key_code == VK_BACK)
        {
            if (text_.size() > 0 && cursor_position_ > 0)
            {
                text_.erase(cursor_position_ - 1, 1);
                cursor_position_--;
                
                elapsed_time_ = 0.f;
                cursor_visible_ = true;
            }
        }
        else if (key_code == VK_HOME)
        {
            cursor_position_ = 0;
            
            elapsed_time_ = 0.f;
            cursor_visible_ = true;
        }
        else if (key_code == VK_END)
        {
            cursor_position_ = text_.size();
            
            elapsed_time_ = 0.f;
            cursor_visible_ = true;
        }
        else if (key_code == VK_DELETE)
        {
            if (text_.size() > 0 && cursor_position_ < text_.size())
            {
                text_.erase(cursor_position_, 1);
                
                elapsed_time_ = 0.f;
                cursor_visible_ = true;
            }
        }
    }
}

void EditableTextBox::OnInputText(wchar_t character)
{
    Widget::OnInputText(character);

    text_.insert(cursor_position_, 1, character);
    cursor_position_++;

    Renderer* renderer = Renderer::Get();
    renderer->GetTextAdvances(/*rect_, */text_, L"Nanum18", advances_);

    OnTextChanged.Execute(std::move(character));

    // Space
    if (character == 32)
    {
        elapsed_time_ = 0.f;
        cursor_visible_ = true;
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<EditableTextBox>("EditableTextBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
