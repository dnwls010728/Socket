#include "pch.h"
#include "Shaders.h"

#include <d3dcompiler.h>

#include "Math/Color.h"
#include "Misc/EngineMacros.h"

VertexShader::VertexShader() :
    shader_(nullptr),
    shader_buffer_(nullptr),
    input_layout_(nullptr)
{
}

bool VertexShader::Create(const std::wstring& kPath, const D3D11_INPUT_ELEMENT_DESC* kLayout, UINT layout_count)
{
    HRESULT hr = D3DReadFileToBlob(kPath.c_str(), shader_buffer_.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = Renderer::Get()->GetDevice()->CreateVertexShader(shader_buffer_->GetBufferPointer(), shader_buffer_->GetBufferSize(), nullptr,
                                          shader_.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = Renderer::Get()->GetDevice()->CreateInputLayout(kLayout, layout_count, shader_buffer_->GetBufferPointer(),
                                         shader_buffer_->GetBufferSize(), input_layout_.GetAddressOf());
    if (FAILED(hr)) return false;

    return true;
}

void VertexShader::BindShader()
{
    Renderer::Get()->GetDeviceContext()->IASetInputLayout(input_layout_.Get());
    Renderer::Get()->GetDeviceContext()->VSSetShader(shader_.Get(), nullptr, 0);
}

void VertexShader::BindParameters()
{
    UpdateParameters();
}

PixelShader::PixelShader() :
    shader_(nullptr),
    shader_buffer_(nullptr)
{
}

bool PixelShader::Create(const std::wstring& kPath)
{
    HRESULT hr = D3DReadFileToBlob(kPath.c_str(), shader_buffer_.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = Renderer::Get()->GetDevice()->CreatePixelShader(shader_buffer_->GetBufferPointer(), shader_buffer_->GetBufferSize(), nullptr,
                                         shader_.GetAddressOf());
    if (FAILED(hr)) return false;

    return true;
}

void PixelShader::BindShader()
{
    Renderer::Get()->GetDeviceContext()->PSSetShader(shader_.Get(), nullptr, 0);
}

void PixelShader::BindParameters()
{
    UpdateParameters();
}

DefaultVertexShader::DefaultVertexShader()
{
    CHECK(constant_buffer_.Create());

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    bool r = Create(L".\\Content\\Shaders\\DefaultVertexShader.cso", layout, ARRAYSIZE(layout));
    CHECK(r);
}

void DefaultVertexShader::SetWorldMatrix(const DirectX::XMMATRIX& kMat)
{
    constant_buffer_.GetBufferData().mat = kMat;
}

void DefaultVertexShader::SetUVOffset(const DirectX::XMFLOAT2& kOffset)
{
    constant_buffer_.GetBufferData().uv_offset = kOffset;
}

void DefaultVertexShader::SetUVScale(const DirectX::XMFLOAT2& kScale)
{
    constant_buffer_.GetBufferData().uv_scale = kScale;
}

void DefaultVertexShader::UpdateParameters()
{
    VertexShader::UpdateParameters();

    Renderer::Get()->GetDeviceContext()->VSSetConstantBuffers(0, 1, constant_buffer_.GetResourceAddress());
    constant_buffer_.UpdateBuffer();
}

DefaultPixelShader::DefaultPixelShader()
{
    CHECK(constant_buffer_.Create());

    bool r = Create(L".\\Content\\Shaders\\DefaultPixelShader.cso");
    CHECK(r);
}

void DefaultPixelShader::EnableTexture(BOOL has_texture)
{
    constant_buffer_.GetBufferData().has_texture = has_texture;
}

void DefaultPixelShader::SetColor(const Math::Color& kColor)
{
    constant_buffer_.GetBufferData().r = kColor.r / 255.f;
    constant_buffer_.GetBufferData().g = kColor.g / 255.f;
    constant_buffer_.GetBufferData().b = kColor.b / 255.f;
    constant_buffer_.GetBufferData().a = kColor.a / 255.f;
}

void DefaultPixelShader::UpdateParameters()
{
    PixelShader::UpdateParameters();

    Renderer::Get()->GetDeviceContext()->PSSetConstantBuffers(0, 1, constant_buffer_.GetResourceAddress());
    constant_buffer_.UpdateBuffer();
}

PostProcessShader::PostProcessShader()
{
    CHECK(constant_buffer_.Create());
    bool r = Create(L".\\Content\\Shaders\\PostProcess.cso");
    CHECK(r);
}

void PostProcessShader::SetResolution(const DirectX::XMFLOAT2& res)
{
    constant_buffer_.GetBufferData().resolution = res;
}

void PostProcessShader::SetBlurRadius(float radius)
{
    constant_buffer_.GetBufferData().blur_radius = radius;
}

void PostProcessShader::SetVignette(float strength)
{
    constant_buffer_.GetBufferData().vignette_strength = strength;
}

void PostProcessShader::SetGamma(float gamma)
{
    constant_buffer_.GetBufferData().gamma = gamma;
}

void PostProcessShader::UseGrayscale(BOOL use)
{
    constant_buffer_.GetBufferData().use_grayscale = use;
}

void PostProcessShader::UpdateParameters()
{
    PixelShader::UpdateParameters();

    Renderer::Get()->GetDeviceContext()->PSSetConstantBuffers(0, 1, constant_buffer_.GetResourceAddress());
    constant_buffer_.UpdateBuffer();
}
