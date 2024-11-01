#pragma once

namespace Math
{
    struct Vector2;
}

class Text;
class Image;

class Slot
{
public:
    Slot(class Widget* parent, const Math::Vector2& kPosition, int slot_index);
    ~Slot() = default;

private:
    int slot_index_;
    
    Image* slot_image_;
    Image* item_image_;

    Text* item_count_text_;
    
};
