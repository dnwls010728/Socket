#include "pch.h"
#include "EditorMap.h"

#include "Actors/Editor/Editor.h"

EditorMap::EditorMap(const std::wstring& name) :
    Level(name)
{
}

void EditorMap::Load()
{
    Level::Load();

    AddActor<Editor>(Editor::StaticClass(), L"Editor");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<EditorMap>("EditorMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
