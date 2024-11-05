#include "pch.h"
#include "Editor.h"

#include <ShObjIdl.h>

#include "imgui/imgui.h"
#include "Windows/DX/UITexture.h"

Editor::Editor(const std::wstring& kName) :
    Actor(kName),
    loaded_texture_(nullptr)
{
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    if (ImGui::Begin("Editor"))
    {
        if (ImGui::Button("Load File"))
        {
            IFileOpenDialog* file_open;
            HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&file_open));
            if (SUCCEEDED(hr))
            {
                hr = file_open->Show(NULL);
                if (SUCCEEDED(hr))
                {
                    IShellItem* item;
                    hr = file_open->GetResult(&item);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR file_path;
                        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);
                        if (SUCCEEDED(hr))
                        {
                            if (loaded_texture_) loaded_texture_.reset();
                            loaded_texture_ = std::make_unique<Texture>();

                            if (loaded_texture_->Load(file_path))
                            {
                            }
                            
                            CoTaskMemFree(file_path);
                        }
                        
                        item->Release();
                    }
                }
                
                file_open->Release();
            }
        }

        if (loaded_texture_)
        {
            ImGui::Image(loaded_texture_->resource_view_.Get(), {static_cast<float>(loaded_texture_->GetWidth()), static_cast<float>(loaded_texture_->GetHeight())});
        }
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
