#include "pch.h"
#include "Renderer.h"

#include "OutlineRenderer.h"
#include "Shaders.h"
#include "UISprite.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Windows/WindowsWindow.h"

Renderer::Renderer() :
    d3d_device_(nullptr),
    d3d_device_context_(nullptr),
    d2d_factory_(nullptr),
    dwrite_factory_(nullptr),
    dwrite_font_collection_(nullptr),
    wic_imaging_factory_(nullptr),
    viewports_(),
    d2d_viewports_(),
    current_viewport_(nullptr),
    current_d2d_viewport_(nullptr),
    
    font_set_builder_(nullptr),
    text_formats_()
{
}

bool Renderer::Init()
{
    if (!CreateDevice()) return false;

    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory_.GetAddressOf());
    if (FAILED(hr)) return false;
    
    if (!CreateDWrite()) return false;

    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wic_imaging_factory_.GetAddressOf()));
    if (FAILED(hr)) return false;

    // TEST
    if (!CreateRenderToTexture()) return false;
    if (!CreatePostProcessResources()) return false;

    return true;
}

bool Renderer::CreateDevice()
{
    constexpr D3D_FEATURE_LEVEL kFeatureLevel[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1
    };

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        kFeatureLevel,
        ARRAYSIZE(kFeatureLevel),
        D3D11_SDK_VERSION,
        d3d_device_.GetAddressOf(),
        nullptr,
        d3d_device_context_.GetAddressOf()
    );

    if (FAILED(hr)) return false;

    return true;
}

bool Renderer::CreateDWrite()
{
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5),
                                     reinterpret_cast<IUnknown**>(dwrite_factory_.GetAddressOf()));
    if (FAILED(hr)) return false;

    // Begin Font Load
    hr = dwrite_factory_->CreateFontSetBuilder(font_set_builder_.GetAddressOf());
    if (FAILED(hr)) return false;

    if (!AddFont(L".\\Content\\Fonts\\Silver.ttf")) return false;
    if (!AddFont(L".\\Content\\Fonts\\NanumBarunGothic.ttf")) return false;

    // End Font Load
    Microsoft::WRL::ComPtr<IDWriteFontSet> font_set;
    hr = font_set_builder_->CreateFontSet(font_set.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = dwrite_factory_->CreateFontCollectionFromFontSet(font_set.Get(), dwrite_font_collection_.GetAddressOf());
    if (FAILED(hr)) return false;

    // Add TextFormat
    if (!AddTextFormat(L"Silver", 24.f)) return false;
    if (!AddTextFormat(L"NanumBarunGothic", 12.f)) return false;
    if (!AddTextFormat(L"NanumBarunGothic", 16.f)) return false;
    if (!AddTextFormat(L"NanumBarunGothic", 18.f)) return false;

    return SUCCEEDED(hr);
}

bool Renderer::CreateViewport(std::shared_ptr<WindowsWindow> window, Math::Vector2 window_size)
{
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swap_chain_desc.BufferDesc.Width = window_size.x;
    swap_chain_desc.BufferDesc.Height = window_size.y;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 2;
    swap_chain_desc.OutputWindow = window->GetHWnd();
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_desc.Flags = 0;

    Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device;
    HRESULT hr = d3d_device_->QueryInterface(IID_PPV_ARGS(dxgi_device.GetAddressOf()));
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IDXGIAdapter> dxgi_adapter;
    hr = dxgi_device->GetParent(IID_PPV_ARGS(dxgi_adapter.GetAddressOf()));
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IDXGIFactory> dxgi_factory;
    hr = dxgi_adapter->GetParent(IID_PPV_ARGS(dxgi_factory.GetAddressOf()));
    if (FAILED(hr)) return false;

    Viewport viewport;

    hr = dxgi_factory->CreateSwapChain(d3d_device_.Get(), &swap_chain_desc, viewport.dxgi_swap_chain.GetAddressOf());
    if (FAILED(hr)) return false;

    // Alt + Enter 키를 눌러 전체 화면으로 전환하는 기능을 비활성화
    hr = dxgi_factory->MakeWindowAssociation(window->GetHWnd(), DXGI_MWA_NO_ALT_ENTER);
    if (FAILED(hr)) return false;

    viewport.d3d_viewport.TopLeftX = 0;
    viewport.d3d_viewport.TopLeftY = 0;
    viewport.d3d_viewport.Width = window_size.x;
    viewport.d3d_viewport.Height = window_size.y;
    viewport.d3d_viewport.MinDepth = 0.0f;
    viewport.d3d_viewport.MaxDepth = 1.0f;

    hr = CreateBackBufferResources(viewport.dxgi_swap_chain, viewport.back_buffer, viewport.d3d_render_target_view);
    if (FAILED(hr)) return false;

    viewport.view_matrix = DirectX::XMMatrixIdentity();
    viewport.projection_matrix = DirectX::XMMatrixOrthographicOffCenterLH(
        0.f, window_size.x, window_size.y, 0.f, 0.f, 1.f);

    viewports_[window.get()] = viewport;
    return true;
}

bool Renderer::CreateD2DViewport(std::shared_ptr<WindowsWindow> window)
{
    Viewport* viewport = FindViewport(window.get());
    if (viewport)
    {
        const uint32_t kDPI = GetDpiForWindow(window->GetHWnd());
        const D2D1_RENDER_TARGET_PROPERTIES kRenderTargetProperties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            kDPI,
            kDPI
        );

        Microsoft::WRL::ComPtr<IDXGISurface> dxgi_back_buffer;
        HRESULT hr = viewport->dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(dxgi_back_buffer.GetAddressOf()));
        if (FAILED(hr)) return false;

        D2DViewport d2d_viewport;
        hr = d2d_factory_->CreateDxgiSurfaceRenderTarget(dxgi_back_buffer.Get(), &kRenderTargetProperties,
                                                         d2d_viewport.d2d_render_target.GetAddressOf());
        if (FAILED(hr)) return false;

        d2d_viewports_[window.get()] = d2d_viewport;

        return true;
    }

    return false;
}

bool Renderer::CreateDepthStencilBuffer(Viewport& viewport)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;

    D3D11_TEXTURE2D_DESC depth_stencil_desc;
    ZeroMemory(&depth_stencil_desc, sizeof(D3D11_TEXTURE2D_DESC));

    depth_stencil_desc.Width = viewport.d3d_viewport.Width;
    depth_stencil_desc.Height = viewport.d3d_viewport.Height;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_desc.SampleDesc.Count = 1;
    depth_stencil_desc.SampleDesc.Quality = 0;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;

    HRESULT hr = d3d_device_->CreateTexture2D(&depth_stencil_desc, nullptr, depth_stencil_buffer.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = d3d_device_->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr,
                                             viewport.depth_stencil_view.GetAddressOf());
    return SUCCEEDED(hr);
}

bool Renderer::ResizeViewport(const std::shared_ptr<WindowsWindow>& kWindow, uint32_t width,
                              uint32_t height)
{
    Viewport* viewport = FindViewport(kWindow.get());
    if (viewport && (viewport->d3d_viewport.Width != width || viewport->d3d_viewport.Height != height))
    {
        d3d_device_context_->OMSetRenderTargets(0, nullptr, nullptr);
        d3d_device_context_->ClearState();
        d3d_device_context_->Flush();

        D2DViewport* d2d_viewport = FindD2DViewport(kWindow.get());
        if (d2d_viewport) d2d_viewport->d2d_render_target.Reset();

        viewport->back_buffer.Reset();
        viewport->d3d_render_target_view.Reset();
        viewport->depth_stencil_view.Reset();

        viewport->d3d_viewport.Width = width;
        viewport->d3d_viewport.Height = height;

        DXGI_SWAP_CHAIN_DESC swap_chain_desc;
        HRESULT hr = viewport->dxgi_swap_chain->GetDesc(&swap_chain_desc);
        if (FAILED(hr)) return false;

        hr = viewport->dxgi_swap_chain->ResizeBuffers(swap_chain_desc.BufferCount, width, height,
                                                      swap_chain_desc.BufferDesc.Format, swap_chain_desc.Flags);
        if (FAILED(hr)) return false;

        if (!CreateBackBufferResources(viewport->dxgi_swap_chain, viewport->back_buffer,
                                       viewport->d3d_render_target_view))
            return false;

#pragma region D2D Resize
        const uint32_t kDPI = GetDpiForWindow(kWindow->GetHWnd());
        const D2D1_RENDER_TARGET_PROPERTIES render_target_properties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            kDPI,
            kDPI
        );

        Microsoft::WRL::ComPtr<IDXGISurface> dxgi_back_buffer;
        hr = viewport->dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(dxgi_back_buffer.GetAddressOf()));
        if (FAILED(hr)) return false;

        hr = d2d_factory_->CreateDxgiSurfaceRenderTarget(dxgi_back_buffer.Get(), &render_target_properties,
                                                         d2d_viewport->d2d_render_target.GetAddressOf());
        return SUCCEEDED(hr);
#pragma endregion
    }

    return false;
}

bool Renderer::AddFont(const std::wstring& kPath)
{
    Microsoft::WRL::ComPtr<IDWriteFontFile> font_file;
    HRESULT hr = dwrite_factory_->CreateFontFileReference(kPath.c_str(), nullptr, font_file.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = font_set_builder_->AddFontFile(font_file.Get());
    return SUCCEEDED(hr);
}

bool Renderer::AddTextFormat(const std::wstring& kName, float size)
{
    HRESULT hr = dwrite_factory_->CreateTextFormat(kName.c_str(), dwrite_font_collection_.Get(),
                                        DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
                                        DWRITE_FONT_STRETCH_NORMAL, size, L"ko-kr",
                                        text_formats_[kName][size].GetAddressOf());
    return SUCCEEDED(hr);
}

bool Renderer::CreateRenderToTexture()
{
    D3D11_TEXTURE2D_DESC texture_desc;
    ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));

    rtt_width_ = 1366;
    rtt_height_ = 768;
    texture_desc.Width = rtt_width_;
    texture_desc.Height = rtt_height_;
    texture_desc.MipLevels = 1;
    texture_desc.ArraySize = 1;
    texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.Usage = D3D11_USAGE_DEFAULT;
    texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture_desc.CPUAccessFlags = 0;
    texture_desc.MiscFlags = 0;

    HRESULT hr = d3d_device_->CreateTexture2D(&texture_desc, nullptr, texture_.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
    ZeroMemory(&rtv_desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

    rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtv_desc.Texture2D.MipSlice = 0;

    hr = d3d_device_->CreateRenderTargetView(texture_.Get(), &rtv_desc, rtv_.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MostDetailedMip = 0;
    srv_desc.Texture2D.MipLevels = 1;

    hr = d3d_device_->CreateShaderResourceView(texture_.Get(), &srv_desc, srv_.GetAddressOf());
    return SUCCEEDED(hr);
}

bool Renderer::CreatePostProcessResources()
{
    // Vertex buffer
    D3D11_BUFFER_DESC vb_desc;
    ZeroMemory(&vb_desc, sizeof(vb_desc));
    vb_desc.ByteWidth = sizeof(DefaultVertex) * 4;
    vb_desc.Usage = D3D11_USAGE_DEFAULT;
    vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    HRESULT hr = d3d_device_->CreateBuffer(&vb_desc, nullptr, post_vertex_buffer_.GetAddressOf());
    if (FAILED(hr)) return false;

    DefaultVertex vertices[4] = {
        {DirectX::XMFLOAT3(-1.f,  1.f, 0.f), DirectX::XMFLOAT4(1.f,1.f,1.f,1.f), DirectX::XMFLOAT2(0.f,0.f)},
        {DirectX::XMFLOAT3( 1.f,  1.f, 0.f), DirectX::XMFLOAT4(1.f,1.f,1.f,1.f), DirectX::XMFLOAT2(1.f,0.f)},
        {DirectX::XMFLOAT3( 1.f, -1.f, 0.f), DirectX::XMFLOAT4(1.f,1.f,1.f,1.f), DirectX::XMFLOAT2(1.f,1.f)},
        {DirectX::XMFLOAT3(-1.f, -1.f, 0.f), DirectX::XMFLOAT4(1.f,1.f,1.f,1.f), DirectX::XMFLOAT2(0.f,1.f)}
    };
    d3d_device_context_->UpdateSubresource(post_vertex_buffer_.Get(), 0, nullptr, vertices, 0, 0);

    // Index buffer
    D3D11_BUFFER_DESC ib_desc;
    ZeroMemory(&ib_desc, sizeof(ib_desc));
    ib_desc.ByteWidth = sizeof(uint32_t) * 6;
    ib_desc.Usage = D3D11_USAGE_DEFAULT;
    ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    hr = d3d_device_->CreateBuffer(&ib_desc, nullptr, post_index_buffer_.GetAddressOf());
    if (FAILED(hr)) return false;

    uint32_t indices[6] = {0,1,2,0,2,3};
    d3d_device_context_->UpdateSubresource(post_index_buffer_.Get(), 0, nullptr, indices, 0, 0);

    // Sampler
    D3D11_SAMPLER_DESC samp_desc;
    ZeroMemory(&samp_desc, sizeof(samp_desc));
    samp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samp_desc.MinLOD = 0;
    samp_desc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = d3d_device_->CreateSamplerState(&samp_desc, post_sampler_state_.GetAddressOf());
    if (FAILED(hr)) return false;

    post_vertex_shader_ = std::make_shared<DefaultVertexShader>();
    post_pixel_shader_ = std::make_shared<DefaultPixelShader>();

    return post_vertex_shader_ && post_pixel_shader_;
}

void Renderer::BeginRTT()
{
    constexpr float kClearColor[4] = {
        49.f / 255.f,
        77.f / 255.f,
        121.f / 255.f,
        1.f
    };

    d3d_device_context_->ClearRenderTargetView(rtv_.Get(), kClearColor);
    d3d_device_context_->OMSetRenderTargets(1, rtv_.GetAddressOf(), nullptr);
}

void Renderer::EndRTT()
{
    d3d_device_context_->OMSetRenderTargets(0, nullptr, nullptr);
}

void Renderer::DrawPostProcess(float blur_radius, float vignette_strength, float gamma)
{
    if (!post_vertex_shader_ || !post_pixel_shader_) return;

    post_vertex_shader_->BindShader();
    post_vertex_shader_->SetWorldMatrix(DirectX::XMMatrixIdentity());
    post_vertex_shader_->SetUVOffset({0.f,0.f});
    post_vertex_shader_->SetUVScale({1.f,1.f});
    post_vertex_shader_->BindParameters();

    post_pixel_shader_->BindShader();
    // post_pixel_shader_->SetResolution({static_cast<float>(rtt_width_), static_cast<float>(rtt_height_)});
    // post_pixel_shader_->SetBlurRadius(blur_radius);
    // post_pixel_shader_->SetVignette(vignette_strength);
    // post_pixel_shader_->SetGamma(gamma);
    post_pixel_shader_->SetColor(Math::Color::White);
    post_pixel_shader_->BindParameters();

    d3d_device_context_->PSSetSamplers(0, 1, post_sampler_state_.GetAddressOf());
    ID3D11ShaderResourceView* srv = srv_.Get();
    d3d_device_context_->PSSetShaderResources(0, 1, &srv);

    UINT stride = sizeof(DefaultVertex);
    UINT offset = 0;
    ID3D11Buffer* vb = post_vertex_buffer_.Get();
    d3d_device_context_->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
    d3d_device_context_->IASetIndexBuffer(post_index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
    d3d_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    d3d_device_context_->DrawIndexed(6, 0, 0);

    ID3D11ShaderResourceView* null_srv[1] = { nullptr };
    d3d_device_context_->PSSetShaderResources(0, 1, null_srv);
}

Viewport* Renderer::FindViewport(WindowsWindow* window)
{
    const auto it = viewports_.find(window);
    if (it == viewports_.end()) return nullptr;

    return &it->second;
}

D2DViewport* Renderer::FindD2DViewport(WindowsWindow* window)
{
    const auto it = d2d_viewports_.find(window);
    if (it == d2d_viewports_.end()) return nullptr;

    return &it->second;
}

void Renderer::BeginRender(const std::shared_ptr<WindowsWindow>& kWindow)
{
    current_viewport_ = FindViewport(kWindow.get());
    CHECK_IF(current_viewport_, L"Not found viewport for window.");

    constexpr float kClearColor[4] = {
        49.f / 255.f,
        77.f / 255.f,
        121.f / 255.f,
        1.f
    };

    d3d_device_context_->ClearRenderTargetView(current_viewport_->d3d_render_target_view.Get(), kClearColor);
    d3d_device_context_->ClearDepthStencilView(current_viewport_->depth_stencil_view.Get(),
                                               D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    d3d_device_context_->RSSetViewports(1, &current_viewport_->d3d_viewport);

    ID3D11RenderTargetView* render_target_view = current_viewport_->d3d_render_target_view.Get();
    ID3D11DepthStencilView* depth_stencil_view = current_viewport_->depth_stencil_view.Get();

    d3d_device_context_->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
}

void Renderer::EndRender()
{
    CHECK_IF(current_viewport_, L"Not Set current viewport.");

    d3d_device_context_->OMSetRenderTargets(0, nullptr, nullptr);
    current_viewport_->dxgi_swap_chain->Present(EngineSettings::Get()->IsUseVSync(), 0);

    current_viewport_ = nullptr;
}

void Renderer::BeginRenderD2D(const std::shared_ptr<WindowsWindow>& kWindow)
{
    current_d2d_viewport_ = FindD2DViewport(kWindow.get());
    CHECK_IF(current_d2d_viewport_, L"Not found D2D viewport for window.");

    current_d2d_viewport_->d2d_render_target->BeginDraw();
}

void Renderer::EndRenderD2D()
{
    current_d2d_viewport_->d2d_render_target->EndDraw();
    current_d2d_viewport_ = nullptr;
}

void Renderer::BeginLayer(const Math::Vector2& position, const Math::Vector2& size) const
{
    Microsoft::WRL::ComPtr<ID2D1Layer> layer;
    current_d2d_viewport_->d2d_render_target->CreateLayer(nullptr, &layer);

    D2D1_RECT_F clipRect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);
    current_d2d_viewport_->d2d_render_target->PushLayer(
        D2D1::LayerParameters(
            clipRect,
            nullptr,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
            D2D1::IdentityMatrix(),
            1.0f,
            nullptr,
            D2D1_LAYER_OPTIONS_NONE),
        layer.Get()
    );
}

void Renderer::EndLayer()
{
    current_d2d_viewport_->d2d_render_target->PopLayer();
}

void Renderer::ChangeResolution(WindowsWindow* window, uint32_t width, uint32_t height, bool is_fullscreen)
{
    Viewport* viewport = FindViewport(window);
    if (!viewport) return;

    viewport->dxgi_swap_chain->SetFullscreenState(is_fullscreen, nullptr);

    DXGI_MODE_DESC mode_desc;
    ZeroMemory(&mode_desc, sizeof(DXGI_MODE_DESC));

    mode_desc.Width = width;
    mode_desc.Height = height;
    mode_desc.RefreshRate.Numerator = 60;
    mode_desc.RefreshRate.Denominator = 1;
    mode_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    mode_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    mode_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    viewport->dxgi_swap_chain->ResizeTarget(&mode_desc);
}

Math::Vector2 Renderer::ScreenToWorld(const Math::Vector2& screen_position)
{
    Viewport* viewport = FindViewport(World::Get()->GetWindow());
    if (!viewport) return Math::Vector2::Zero();

    float x = (screen_position.x / viewport->d3d_viewport.Width) * 2.f - 1.f;
    float y = 1.f - (screen_position.y / viewport->d3d_viewport.Height) * 2.f;

    DirectX::XMFLOAT3 clip = { x, y, 0.f };
    
    DirectX::XMFLOAT3 screen;
    DirectX::XMStoreFloat3(&screen, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&clip), DirectX::XMMatrixInverse(nullptr, viewport->projection_matrix)));
    
    DirectX::XMFLOAT3 world;
    DirectX::XMStoreFloat3(&world, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&screen), DirectX::XMMatrixInverse(nullptr, viewport->view_matrix)));

    return { world.x, world.y };
}

Math::Vector2 Renderer::WorldToScreen(const Math::Vector2& world_position)
{
    Viewport* viewport = FindViewport(World::Get()->GetWindow());
    if (!viewport) return Math::Vector2::Zero();
    
    DirectX::XMFLOAT3 world = { world_position.x, world_position.y, 0.f };
    
    DirectX::XMFLOAT3 screen;
    DirectX::XMStoreFloat3(&screen, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&world), viewport->view_matrix));
    
    DirectX::XMFLOAT3 clip;
    DirectX::XMStoreFloat3(&clip, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&screen), viewport->projection_matrix));

    float x = (clip.x + 1.f) * 0.5f * viewport->d3d_viewport.Width;
    float y = (1.f - clip.y) * 0.5f * viewport->d3d_viewport.Height;

    return { x, y };
}

void Renderer::DrawBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color, float stroke)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    D2D1_MATRIX_3X2_F transform;
    d2d_viewport->d2d_render_target->GetTransform(&transform);

    const D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = current_d2d_viewport_->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf()
    );
    
    if (FAILED(hr)) return;

    D2D1_POINT_2F center = D2D1::Point2F(position.x, position.y);
    d2d_viewport->d2d_render_target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, center));

    d2d_viewport->d2d_render_target->DrawRectangle(rect, brush.Get(), stroke);
    d2d_viewport->d2d_render_target->SetTransform(transform);
}

void Renderer::DrawSolidBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    D2D1_MATRIX_3X2_F transform;
    d2d_viewport->d2d_render_target->GetTransform(&transform);

    const D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = current_d2d_viewport_->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf()
    );
    
    if (FAILED(hr)) return;

    D2D1_POINT_2F center = D2D1::Point2F(position.x, position.y);
    d2d_viewport->d2d_render_target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, center));

    d2d_viewport->d2d_render_target->FillRectangle(rect, brush.Get());
    d2d_viewport->d2d_render_target->SetTransform(transform);
}

void Renderer::DrawGradientSolidBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& start_color, const Math::Color& end_color, GradientDirection direction)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    D2D1_MATRIX_3X2_F transform;
    d2d_viewport->d2d_render_target->GetTransform(&transform);

    const D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);

    D2D1_GRADIENT_STOP gradient_stop[2];
    gradient_stop[0].position = 0.f;
    gradient_stop[0].color = D2D1::ColorF(start_color.r / 255.f, start_color.g / 255.f, start_color.b / 255.f, start_color.a / 255.f);
    gradient_stop[1].position = 1.f;
    gradient_stop[1].color = D2D1::ColorF(end_color.r / 255.f, end_color.g / 255.f, end_color.b / 255.f, end_color.a / 255.f);

    Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> gradient_stop_collection;
    HRESULT hr = d2d_viewport->d2d_render_target->CreateGradientStopCollection(
        gradient_stop,
        2,
        D2D1_GAMMA_2_2,
        D2D1_EXTEND_MODE_CLAMP,
        gradient_stop_collection.GetAddressOf()
    );
    if (FAILED(hr)) return;

    D2D1_POINT_2F end_point = { position.x, position.y };
    if (EnumHasAnyFlags(direction, GradientDirection::kHorizontal))
        end_point.x = position.x + size.x;
    if (EnumHasAnyFlags(direction, GradientDirection::kVertical))
        end_point.y = position.y + size.y;
    
    Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> brush;
    hr = d2d_viewport->d2d_render_target->CreateLinearGradientBrush(
        D2D1::LinearGradientBrushProperties(
            {position.x, position.y},
            end_point
        ),
        gradient_stop_collection.Get(),
        brush.GetAddressOf()
    );
    if (FAILED(hr)) return;

    D2D1_POINT_2F center = D2D1::Point2F(position.x, position.y);
    d2d_viewport->d2d_render_target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, center));

    d2d_viewport->d2d_render_target->FillRectangle(rect, brush.Get());
    d2d_viewport->d2d_render_target->SetTransform(transform);
}

void Renderer::DrawRoundBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color, float radius, float stroke)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    D2D1_MATRIX_3X2_F transform;
    d2d_viewport->d2d_render_target->GetTransform(&transform);

    const D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = current_d2d_viewport_->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf()
    );
    
    if (FAILED(hr)) return;

    D2D1_POINT_2F center = D2D1::Point2F(position.x, position.y);
    d2d_viewport->d2d_render_target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, center));

    d2d_viewport->d2d_render_target->DrawRoundedRectangle(D2D1::RoundedRect(rect, radius, radius), brush.Get(), stroke);
    d2d_viewport->d2d_render_target->SetTransform(transform);
}

void Renderer::DrawSolidRoundBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color, float radius)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    D2D1_MATRIX_3X2_F transform;
    d2d_viewport->d2d_render_target->GetTransform(&transform);

    const D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = current_d2d_viewport_->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf()
    );
    
    if (FAILED(hr)) return;

    D2D1_POINT_2F center = D2D1::Point2F(position.x, position.y);
    d2d_viewport->d2d_render_target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, center));

    d2d_viewport->d2d_render_target->FillRoundedRectangle(D2D1::RoundedRect(rect, radius, radius), brush.Get());
    d2d_viewport->d2d_render_target->SetTransform(transform);
}

void Renderer::DrawCircle(const Math::Vector2& position, float radius, const Math::Color& color, float stroke)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;
    
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = d2d_viewport->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf());
    if (FAILED(hr)) return;

    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(position.x, position.y), radius, radius);
    d2d_viewport->d2d_render_target->DrawEllipse(ellipse, brush.Get(), stroke);
}

void Renderer::DrawSolidCircle(const Math::Vector2& position, float radius, const Math::Color& color)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;
    
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = d2d_viewport->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf());
    if (FAILED(hr)) return;

    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(position.x, position.y), radius, radius);
    d2d_viewport->d2d_render_target->FillEllipse(ellipse, brush.Get());
}

void Renderer::DrawString(const std::wstring& string, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color, const std::wstring& font_name, float font_size, DWRITE_TEXT_ALIGNMENT text_alignment, DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment)
{
    // 추후 Window 얻는 방식 변경
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    D2D1_MATRIX_3X2_F transform;
    d2d_viewport->d2d_render_target->GetTransform(&transform);

    const D2D1_RECT_F rect = D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y);

    Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format = GetTextFormat(font_name, font_size);
    if (!text_format) return;

    text_format->SetTextAlignment(text_alignment);
    text_format->SetParagraphAlignment(paragraph_alignment);

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = d2d_viewport->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf()
    );
    
    if (FAILED(hr)) return;
    
    D2D1_POINT_2F center = D2D1::Point2F(position.x, position.y);
    d2d_viewport->d2d_render_target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, center));

    d2d_viewport->d2d_render_target->DrawTextW(
        string.c_str(), static_cast<UINT32>(string.size()),
        text_format.Get(), rect, brush.Get()
    );
    d2d_viewport->d2d_render_target->SetTransform(transform);
}

void Renderer::DrawLine(const Math::Vector2& start, const Math::Vector2& end, const Math::Color& color, float stroke)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = d2d_viewport->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f),
        brush.GetAddressOf());
    if (FAILED(hr)) return;

    d2d_viewport->d2d_render_target->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), brush.Get(), stroke);
}

void Renderer::DrawStringWithOutline(const std::wstring& string, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& outline_color, const Math::Color& fill_color, float stroke, const std::wstring& font_name, float font_size, DWRITE_TEXT_ALIGNMENT text_alignment, DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;
    
    Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format = GetTextFormat(font_name, font_size);
    if (!text_format) return;

    Microsoft::WRL::ComPtr<IDWriteTextLayout> text_layout;
    HRESULT hr = dwrite_factory_->CreateTextLayout(string.c_str(), static_cast<UINT32>(string.size()), text_format.Get(), size.x, size.y, text_layout.GetAddressOf());
    if (FAILED(hr)) return;

    text_layout->SetTextAlignment(text_alignment);
    text_layout->SetParagraphAlignment(paragraph_alignment);
    
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> outline_brush;
    hr = current_d2d_viewport_->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(outline_color.r / 255.f, outline_color.g / 255.f, outline_color.b / 255.f, outline_color.a / 255.f),
        outline_brush.GetAddressOf()
    );
    if (FAILED(hr)) return;
    
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fill_brush;
    hr = current_d2d_viewport_->d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(fill_color.r / 255.f, fill_color.g / 255.f, fill_color.b / 255.f, fill_color.a / 255.f),
        fill_brush.GetAddressOf()
    );
    if (FAILED(hr)) return;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> device_context;
    hr = d2d_viewport->d2d_render_target.As(&device_context);
    if (FAILED(hr)) return;
    
    OutlineRenderer renderer(outline_brush, fill_brush, stroke);
    text_layout->Draw(device_context.Get(), &renderer, position.x, position.y);
}

void Renderer::DrawSimpleSprite(const UISprite* ui_sprite, uint64_t frame_index, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;
    
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> device_context;
    HRESULT hr = d2d_viewport->d2d_render_target.As(&device_context);
    if (FAILED(hr)) return;
    
    D2D1_MATRIX_3X2_F transform;
    device_context->GetTransform(&transform);

    const UISpriteFrame& frame = ui_sprite->frames_[frame_index];

    Microsoft::WRL::ComPtr<ID2D1Effect> effect;
    hr = device_context->CreateEffect(CLSID_D2D1ColorMatrix, &effect);
    if (FAILED(hr)) return;
    
    effect->SetInput(0, ui_sprite->bitmap_.Get());

    float a = Math::Clamp01(color.a / 255.f);
    float r = (color.r / 255.f) * a;
    float g = (color.g / 255.f) * a;
    float b = (color.b / 255.f) * a;

    D2D1_MATRIX_5X4_F color_matrix = {
        r, 0.f, 0.f, 0.f,
        0.f, g, 0.f, 0.f,
        0.f, 0.f, b, 0.f,
        0.f, 0.f, 0.f, a,
        0.f, 0.f, 0.f, 0.f
    };

    effect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, color_matrix);
    effect->SetValue(D2D1_COLORMATRIX_PROP_ALPHA_MODE, D2D1_COLORMATRIX_ALPHA_MODE_PREMULTIPLIED);
    effect->SetValue(D2D1_COLORMATRIX_PROP_CLAMP_OUTPUT, TRUE);

    D2D1_INTERPOLATION_MODE filter_mode = D2D1_INTERPOLATION_MODE_LINEAR;
    if (ui_sprite->filter_mode_ == UISprite::FilterMode::kPoint) filter_mode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

    const D2D1_RECT_F dest_rect = { position.x, position.y, position.x + size.x, position.y + size.y };
    const D2D1_RECT_F src_rect = { frame.offset.x, frame.offset.y, frame.offset.x + frame.size.x, frame.offset.y + frame.size.y };
    const D2D1_RECT_F src_dip = GetDip(ui_sprite->bitmap_.Get(), src_rect);

    const float dist_width = dest_rect.right - dest_rect.left;
    const float dist_height = dest_rect.bottom - dest_rect.top;
    const float src_width = src_dip.right - src_dip.left;
    const float src_height = src_dip.bottom - src_dip.top;

    if (dist_width <= 0.f || dist_height <= 0.f || src_width <= 0.f || src_height <= 0.f)
    {
        device_context->SetTransform(transform);
        return;
    }

    D2D1_MATRIX_3X2_F matrix =
        D2D1::Matrix3x2F::Scale(dist_width / src_width, dist_height / src_height) *
        D2D1::Matrix3x2F::Translation(dest_rect.left, dest_rect.top) *
        transform;

    device_context->SetTransform(matrix);
    device_context->DrawImage(effect.Get(), nullptr, &src_dip, filter_mode);
    
    device_context->SetTransform(transform);
}

void Renderer::DrawSlicedSprite(const UISprite* ui_sprite, uint64_t frame_index, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color)
{
    D2DViewport* d2d_viewport = FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> device_context;
    HRESULT hr = d2d_viewport->d2d_render_target.As(&device_context);
    if (FAILED(hr)) return;
    
    D2D1_MATRIX_3X2_F transform;
    device_context->GetTransform(&transform);

    const UISpriteFrame& frame = ui_sprite->frames_[frame_index];
    
    Microsoft::WRL::ComPtr<ID2D1Effect> effect;
    hr = device_context->CreateEffect(CLSID_D2D1ColorMatrix, &effect);
    if (FAILED(hr)) return;
    
    effect->SetInput(0, ui_sprite->bitmap_.Get());

    float a = Math::Clamp01(color.a / 255.f);
    float r = (color.r / 255.f) * a;
    float g = (color.g / 255.f) * a;
    float b = (color.b / 255.f) * a;

    D2D1_MATRIX_5X4_F color_matrix = {
        r, 0.f, 0.f, 0.f,
        0.f, g, 0.f, 0.f,
        0.f, 0.f, b, 0.f,
        0.f, 0.f, 0.f, a,
        0.f, 0.f, 0.f, 0.f
    };

    effect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, color_matrix);
    effect->SetValue(D2D1_COLORMATRIX_PROP_ALPHA_MODE, D2D1_COLORMATRIX_ALPHA_MODE_PREMULTIPLIED);
    effect->SetValue(D2D1_COLORMATRIX_PROP_CLAMP_OUTPUT, TRUE);

    D2D1_INTERPOLATION_MODE filter_mode = D2D1_INTERPOLATION_MODE_LINEAR;
    if (ui_sprite->filter_mode_ == UISprite::FilterMode::kPoint) filter_mode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

    auto DrawSlice = [&](const D2D1_RECT_F& dest_dip, const D2D1_RECT_F& src_px)
    {
        const D2D1_RECT_F src_dip = GetDip(ui_sprite->bitmap_.Get(), src_px);

        const float dist_width = dest_dip.right - dest_dip.left;
        const float dist_height = dest_dip.bottom - dest_dip.top;
        const float src_width = src_dip.right - src_dip.left;
        const float src_height = src_dip.bottom - src_dip.top;

        if (dist_width <= 0.f || dist_height <= 0.f || src_width <= 0.f || src_height <= 0.f) return;

        D2D1_MATRIX_3X2_F matrix =
            D2D1::Matrix3x2F::Scale(dist_width / src_width, dist_height / src_height) *
            D2D1::Matrix3x2F::Translation(dest_dip.left, dest_dip.top) *
            transform;

        device_context->SetTransform(matrix);
        device_context->DrawImage(effect.Get(), nullptr, &src_dip, filter_mode);
    };

    // Top Left
    {
        const D2D1_RECT_F dest = {
            position.x, position.y,
            position.x + frame.border_min.x, position.y + frame.border_min.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x, frame.offset.y,
            frame.offset.x + frame.border_min.x, frame.offset.y + frame.border_min.y
        };
        DrawSlice(dest, src);
    }

    // Top Center
    {
        const D2D1_RECT_F dest = {
            position.x + frame.border_min.x, position.y,
            position.x + size.x - frame.border_max.x, position.y + frame.border_min.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x + frame.border_min.x, frame.offset.y,
            frame.offset.x + frame.size.x - frame.border_max.x, frame.offset.y + frame.border_min.y
        };
        DrawSlice(dest, src);
    }

    // Top Right
    {
        const D2D1_RECT_F dest = {
            position.x + size.x - frame.border_max.x, position.y,
            position.x + size.x, position.y + frame.border_min.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x + frame.size.x - frame.border_max.x, frame.offset.y,
            frame.offset.x + frame.size.x, frame.offset.y + frame.border_min.y
        };
        DrawSlice(dest, src);
    }

    // Center Left
    {
        const D2D1_RECT_F dest = {
            position.x, position.y + frame.border_min.y,
            position.x + frame.border_min.x, position.y + size.y - frame.border_max.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x, frame.offset.y + frame.border_min.y,
            frame.offset.x + frame.border_min.x, frame.offset.y + frame.size.y - frame.border_max.y
        };
        DrawSlice(dest, src);
    }

    // Center
    {
        const D2D1_RECT_F dest = {
            position.x + frame.border_min.x, position.y + frame.border_min.y,
            position.x + size.x - frame.border_max.x, position.y + size.y - frame.border_max.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x + frame.border_min.x, frame.offset.y + frame.border_min.y,
            frame.offset.x + frame.size.x - frame.border_max.x, frame.offset.y + frame.size.y - frame.border_max.y
        };
        DrawSlice(dest, src);
    }

    // Center Right
    {
        const D2D1_RECT_F dest = {
            position.x + size.x - frame.border_max.x, position.y + frame.border_min.y,
            position.x + size.x, position.y + size.y - frame.border_max.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x + frame.size.x - frame.border_max.x, frame.offset.y + frame.border_min.y,
            frame.offset.x + frame.size.x, frame.offset.y + frame.size.y - frame.border_max.y
        };
        DrawSlice(dest, src);
    }

    // Bottom Left
    {
        const D2D1_RECT_F dest = {
            position.x, position.y + size.y - frame.border_max.y,
            position.x + frame.border_min.x, position.y + size.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x, frame.offset.y + frame.size.y - frame.border_max.y,
            frame.offset.x + frame.border_min.x, frame.offset.y + frame.size.y
        };
        DrawSlice(dest, src);
    }

    // Bottom Center
    {
        const D2D1_RECT_F dest = {
            position.x + frame.border_min.x, position.y + size.y - frame.border_max.y,
            position.x + size.x - frame.border_max.x, position.y + size.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x + frame.border_min.x, frame.offset.y + frame.size.y - frame.border_max.y,
            frame.offset.x + frame.size.x - frame.border_max.x, frame.offset.y + frame.size.y
        };
        DrawSlice(dest, src);
    }

    // Bottom Right
    {
        const D2D1_RECT_F dest = {
            position.x + size.x - frame.border_max.x, position.y + size.y - frame.border_max.y,
            position.x + size.x, position.y + size.y
        };
        const D2D1_RECT_F src = {
            frame.offset.x + frame.size.x - frame.border_max.x, frame.offset.y + frame.size.y - frame.border_max.y,
            frame.offset.x + frame.size.x, frame.offset.y + frame.size.y
        };
        DrawSlice(dest, src);
    }

    device_context->SetTransform(transform);
}

bool Renderer::LoadBitmap(const std::shared_ptr<WindowsWindow>& kWindow, const std::wstring& kFileName, Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap)
{
    D2DViewport* d2d_viewport = FindD2DViewport(kWindow.get());
    if (!d2d_viewport) return false;
    
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    HRESULT hr = wic_imaging_factory_->CreateDecoderFromFilename(kFileName.c_str(), nullptr, GENERIC_READ,
                                                                WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICFormatConverter> format_converter;
    hr = wic_imaging_factory_->CreateFormatConverter(format_converter.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = format_converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
                                       nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) return false;
    
    hr = d2d_viewport->d2d_render_target->CreateBitmapFromWicBitmap(format_converter.Get(), bitmap.GetAddressOf());
    return SUCCEEDED(hr);
}

Microsoft::WRL::ComPtr<IDWriteTextFormat> Renderer::GetTextFormat(const std::wstring& kName, float size)
{
    const auto name_it = text_formats_.find(kName);
    if (name_it == text_formats_.end()) return nullptr;

    const auto size_it = name_it->second.find(size);
    if (size_it != name_it->second.end()) return size_it->second;
    return nullptr;
}

bool Renderer::GetTextAdvances(const std::wstring& kString, const std::wstring& kFontName, float font_size, const Math::Vector2& size, std::vector<float>& advances, std::vector<float>& line_heights)
{
    Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format = GetTextFormat(kFontName, font_size);
    if (!text_format) return false;

    Microsoft::WRL::ComPtr<IDWriteTextLayout> text_layout;
    HRESULT hr = dwrite_factory_->CreateTextLayout(kString.c_str(), static_cast<UINT32>(kString.size()), text_format.Get(), size.x, size.y, text_layout.GetAddressOf());
    if (FAILED(hr)) return false;
    
    std::vector<DWRITE_CLUSTER_METRICS> cluster_metrics;

    uint32_t cluster_count = 0;
    text_layout->GetClusterMetrics(nullptr, 0, &cluster_count);
        
    cluster_metrics.resize(cluster_count);
    text_layout->GetClusterMetrics(cluster_metrics.data(), cluster_count, &cluster_count);

    advances.clear();
        
    for (const auto& cluster : cluster_metrics)
    {
        advances.push_back(cluster.width);
    }

    UINT32 line_count = 0;
    text_layout->GetLineMetrics(nullptr, 0, &line_count);
    std::vector<DWRITE_LINE_METRICS> line_metrics(line_count);
    text_layout->GetLineMetrics(line_metrics.data(), line_count, &line_count);

    line_heights.clear();

    for (const auto& line : line_metrics)
    {
        line_heights.push_back(line.height);
    }

    return true;
}

bool Renderer::GetTextSize(const std::wstring& text, const std::wstring& font_name, float font_size, float& out_width, float& out_height)
{
    Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format = GetTextFormat(font_name, font_size);
    if (!text_format)
        return false;
    
    constexpr float layout_width = 10000.f;
    constexpr float layout_height = 10000.f;

    Microsoft::WRL::ComPtr<IDWriteTextLayout> text_layout;
    HRESULT hr = dwrite_factory_->CreateTextLayout(
        text.c_str(),
        static_cast<UINT32>(text.length()),
        text_format.Get(),
        layout_width,
        layout_height,
        text_layout.GetAddressOf()
    );
    if (FAILED(hr))
        return false;
    
    text_layout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
    
    DWRITE_TEXT_METRICS metrics = {};
    hr = text_layout->GetMetrics(&metrics);
    if (FAILED(hr))
        return false;
    
    out_width = metrics.widthIncludingTrailingWhitespace;
    out_height = metrics.height;

    return true;
}

D2D1_RECT_F Renderer::GetDip(const Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap, const D2D1_RECT_F& rect)
{
    float bmp_dpi_x = 96.f;
    float bmp_dpi_y = 96.f;
    bitmap->GetDpi(&bmp_dpi_x, &bmp_dpi_y);

    float px_to_dip_x = 96.f / bmp_dpi_x;
    float px_to_dip_y = 96.f / bmp_dpi_y;

    return {
        rect.left * px_to_dip_x, rect.top * px_to_dip_y,
        rect.right * px_to_dip_x, rect.bottom * px_to_dip_y
    };
}

bool Renderer::CreateBackBufferResources(Microsoft::WRL::ComPtr<IDXGISwapChain>& dxgi_swap_chain,
                                         Microsoft::WRL::ComPtr<ID3D11Texture2D>& back_buffer,
                                         Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& d3d_render_target_view)
{
    HRESULT hr = dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()));
    if (FAILED(hr)) return false;

    hr = d3d_device_->CreateRenderTargetView(back_buffer.Get(), nullptr, d3d_render_target_view.GetAddressOf());
    return SUCCEEDED(hr);
}

