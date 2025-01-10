#include "pch.h"
#include "EditorMap.h"

#include "Editor/Editor.h"

EditorMap::EditorMap(const std::wstring& kName) :
    Level(kName)
{
}

void EditorMap::Load()
{
    Level::Load();

    Editor* editor = AddActor<Editor>(L"Editor");
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
