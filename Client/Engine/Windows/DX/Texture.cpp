#include "pch.h"
#include "Texture.h"

#include "Renderer.h"
#include "DirectXTK/WICTextureLoader.h"

Texture::Texture() :
    resource_(nullptr),
    resource_view_(nullptr),
    width_(0),
    height_(0),
    vertices_(),
    indices_(),
    wrap_mode_(WrapMode::kReapet),
    filter_mode_(FilterMode::kPoint)
{
    vertices_ = {
        {{0.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 1.f}},
        {{1.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f}},
        {{0.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
        {{1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 0.f}}
    };

    indices_ = {
        0, 1, 2,
        2, 1, 3
    };
}

bool Texture::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);
    
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        Renderer::Get()->GetDevice(),
        kPath.c_str(),
        resource_.GetAddressOf(),
        resource_view_.GetAddressOf()
    );

    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    hr = resource_->QueryInterface<ID3D11Texture2D>(texture.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_TEXTURE2D_DESC texture_desc;
    texture->GetDesc(&texture_desc);

    width_ = texture_desc.Width;
    height_ = texture_desc.Height;

    return true;
}

void Texture::Bind() const
{
    Renderer::Get()->GetDeviceContext()->PSSetShaderResources(0, 1, resource_view_.GetAddressOf());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Texture>("Texture")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
