#include "pch.h"
#include "ShapeBatch.h"

#include "Shape.h"
#include "Texture.h"
#include "Vertex.h"
#include "Misc/EngineMacros.h"

ShapeBatch::ShapeBatch() :
    point_sampler_state_wrap_(nullptr),
    bilinear_sampler_state_wrap_(nullptr),
    bilinear_sampler_state_clamp_(nullptr),
    point_sampler_state_clamp_(nullptr),
    blend_state_(nullptr),
    rasterizer_state_(nullptr),
    depth_stencil_state_(nullptr),
    vertex_buffer_(),
    index_buffer_(),
    vertex_shader_(nullptr),
    pixel_shader_(nullptr)
{
}

bool ShapeBatch::Init()
{
    D3D11_SAMPLER_DESC sampler_desc;
    ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampler_desc.MinLOD = 0;
    sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = Renderer::Get()->GetDevice()->CreateSamplerState(&sampler_desc, point_sampler_state_wrap_.GetAddressOf());
    if (FAILED(hr)) return false;

    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    hr = Renderer::Get()->GetDevice()->CreateSamplerState(&sampler_desc, bilinear_sampler_state_wrap_.GetAddressOf());
    if (FAILED(hr)) return false;

    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    hr = Renderer::Get()->GetDevice()->CreateSamplerState(&sampler_desc, bilinear_sampler_state_clamp_.GetAddressOf());
    if (FAILED(hr)) return false;

    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    hr = Renderer::Get()->GetDevice()->CreateSamplerState(&sampler_desc, point_sampler_state_clamp_.GetAddressOf());
    if (FAILED(hr)) return false;
    
    if (!vertex_buffer_.CreateBuffer(sizeof(DefaultVertex), true, false)) return false;
    if (!index_buffer_.CreateBuffer(true, false)) return false;

    vertex_shader_ = std::make_shared<DefaultVertexShader>();
    pixel_shader_ = std::make_shared<DefaultPixelShader>();

    D3D11_BLEND_DESC blend_desc;
    ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));

    D3D11_RENDER_TARGET_BLEND_DESC render_target_blend_desc;
    ZeroMemory(&render_target_blend_desc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

    render_target_blend_desc.BlendEnable = true;
    render_target_blend_desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    render_target_blend_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    render_target_blend_desc.BlendOp = D3D11_BLEND_OP_ADD;
    render_target_blend_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
    render_target_blend_desc.DestBlendAlpha = D3D11_BLEND_ZERO;
    render_target_blend_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    render_target_blend_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blend_desc.RenderTarget[0] = render_target_blend_desc;

    hr = Renderer::Get()->GetDevice()->CreateBlendState(&blend_desc, blend_state_.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_RASTERIZER_DESC rasterizer_desc;
    ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_NONE;

    hr = Renderer::Get()->GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc;
    ZeroMemory(&depth_stencil_state_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    // depth_stencil_state_desc.DepthEnable = true;
    // depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    // depth_stencil_state_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    
    depth_stencil_state_desc.DepthEnable = false;
    depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depth_stencil_state_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

    hr = Renderer::Get()->GetDevice()->CreateDepthStencilState(&depth_stencil_state_desc, depth_stencil_state_.GetAddressOf());
    return SUCCEEDED(hr);
}

void ShapeBatch::DrawShapes(const std::shared_ptr<WindowsWindow>& kWindow, const std::vector<std::shared_ptr<Shape>>& kShapes)
{
    Viewport* viewport = Renderer::Get()->FindViewport(kWindow.get());
    CHECK(viewport);

    for (auto& shape : kShapes)
    {    
#pragma region 초기화 및 버퍼 업데이트
        // Buffer 메모리 잠금
        void* vertices_ptr = vertex_buffer_.Lock();
        void* indices_ptr = index_buffer_.Lock();

        CopyMemory(vertices_ptr, shape->GetVertices().data(), sizeof(DefaultVertex) * shape->GetVertices().size());
        CopyMemory(indices_ptr, shape->GetIndices().data(), sizeof(uint32_t) * shape->GetIndices().size());

        // Buffer 메모리 잠금 해제
        vertex_buffer_.Unlock();
        index_buffer_.Unlock();
#pragma endregion

#pragma region 쉐이더 및 상태 설정
        vertex_shader_->BindShader();
        pixel_shader_->BindShader();
        
        DirectX::XMMATRIX wvp_matrix = shape->GetWorldMatrix() * viewport->view_matrix * viewport->projection_matrix;
        vertex_shader_->SetWorldMatrix(wvp_matrix);
        vertex_shader_->SetUVOffset({shape->GetUVOffset().x, shape->GetUVOffset().y});
        vertex_shader_->SetUVScale({shape->GetUVScale().x, shape->GetUVScale().y});

        pixel_shader_->EnableTexture(shape->GetTexture() != nullptr);
        pixel_shader_->SetColor(shape->GetColor());
        pixel_shader_->BindParameters();

        Renderer::Get()->GetDeviceContext()->OMSetBlendState(blend_state_.Get(), nullptr, 0xffffffff);
        Renderer::Get()->GetDeviceContext()->RSSetState(rasterizer_state_.Get());
        Renderer::Get()->GetDeviceContext()->OMSetDepthStencilState(depth_stencil_state_.Get(), 0);

        Renderer::Get()->GetDeviceContext()->IASetPrimitiveTopology(shape->GetPrimitiveTopology());
#pragma endregion

#pragma region 드로우 콜
        ID3D11Buffer* buffer = vertex_buffer_.GetResource();
        vertex_shader_->BindParameters();

        constexpr uint32_t kStride = sizeof(DefaultVertex);
        constexpr uint32_t kOffset = 0;

        if (const Texture* texture = shape->GetTexture())
        {
            if (texture->GetWrapMode() == Texture::WrapMode::kClamp)
            {
                if (texture->GetFilterMode() == Texture::FilterMode::kPoint)
                {
                    Renderer::Get()->GetDeviceContext()->PSSetSamplers(0, 1, point_sampler_state_clamp_.GetAddressOf());
                }
                else if (texture->GetFilterMode() == Texture::FilterMode::kBilinear)
                {
                    Renderer::Get()->GetDeviceContext()->PSSetSamplers(0, 1, bilinear_sampler_state_clamp_.GetAddressOf());
                }
            }
            else
            {
                if (texture->GetFilterMode() == Texture::FilterMode::kPoint)
                {
                    Renderer::Get()->GetDeviceContext()->PSSetSamplers(0, 1, point_sampler_state_wrap_.GetAddressOf());
                }
                else if (texture->GetFilterMode() == Texture::FilterMode::kBilinear)
                {
                    Renderer::Get()->GetDeviceContext()->PSSetSamplers(0, 1, bilinear_sampler_state_wrap_.GetAddressOf());
                }
            }
            
            texture->Bind();
        }
        
        Renderer::Get()->GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &kStride, &kOffset);
        Renderer::Get()->GetDeviceContext()->IASetIndexBuffer(index_buffer_.GetResource(), DXGI_FORMAT_R32_UINT, 0);

        if (!shape->GetIndices().empty())
        {
            Renderer::Get()->GetDeviceContext()->DrawIndexed(shape->GetIndices().size(), 0, 0);
        }
        else
        {
            Renderer::Get()->GetDeviceContext()->Draw(shape->GetVertices().size(), 0);
        }
#pragma endregion
    }
}


