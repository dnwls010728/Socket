#include "pch.h"
#include "Editor.h"

#include <commdlg.h>

#include "imgui/imgui.h"

Editor::Editor(const std::wstring& kName) :
    Actor(kName)
{
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    if (ImGui::Begin("Editor"))
    {
    }

    ImGui::End();
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Editor>("Editor")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
