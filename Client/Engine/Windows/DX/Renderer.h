#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")

#include <wrl/client.h>
#include <d3d11.h>
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <map>
#include <vector>

#include "Singleton.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "Windows/WindowsWindow.h"

class PostProcessShader;
class UISprite;
class DefaultPixelShader;
class DefaultVertexShader;

namespace Math
{
    struct Vector2;
    struct Color;
}

class WindowsWindow;

struct Viewport
{
    Microsoft::WRL::ComPtr<IDXGISwapChain> dxgi_swap_chain;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3d_render_target_view;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
    
    D3D11_VIEWPORT d3d_viewport;

    DirectX::XMMATRIX view_matrix;
    DirectX::XMMATRIX projection_matrix;
};

struct D2DViewport
{
    Microsoft::WRL::ComPtr<ID2D1RenderTarget> d2d_render_target;
};

enum class GradientDirection : uint8_t
{
    kHorizontal = (0x01 << 0),
    kVertical = (0x01 << 1),
    kDiagonal = (0x01 << 2)
};

ENUM_CLASS_FLAGS(GradientDirection)

class Renderer : public Singleton<Renderer>
{
public:
    Renderer();
    virtual ~Renderer() override = default;

    bool Init();
    bool CreateDevice();
    bool CreateDWrite();
    bool CreateViewport(std::shared_ptr<WindowsWindow> window, Math::Vector2 window_size);
    bool CreateD2DViewport(std::shared_ptr<WindowsWindow> window);
    bool CreateDepthStencilBuffer(Viewport& viewport);
    bool ResizeViewport(const std::shared_ptr<WindowsWindow>& kWindow, uint32_t width, uint32_t height);
    bool AddFont(const std::wstring& kPath);
    bool AddTextFormat(const std::wstring& kName, float size);
    
    // TEST
    bool CreateRenderToTexture();
    bool CreatePostProcessResources();

    void BeginRTT();
    void EndRTT();
    void DrawPostProcess(float blur_radius, float vignette_strength, float gamma);

    FORCEINLINE const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV() const { return srv_; }

    Viewport* FindViewport(WindowsWindow* window);
    D2DViewport* FindD2DViewport(WindowsWindow* window);

    void BeginRender(const std::shared_ptr<WindowsWindow>& kWindow);
    void EndRender();
    void BeginRenderD2D(const std::shared_ptr<WindowsWindow>& kWindow);
    void EndRenderD2D();
    void BeginLayer(const Math::Vector2& position, const Math::Vector2& size) const;
    void EndLayer();
    void ChangeResolution(WindowsWindow* window, uint32_t width, uint32_t height, bool is_fullscreen = false);
    
    Math::Vector2 ScreenToWorld(const Math::Vector2& screen_position);
    Math::Vector2 WorldToScreen(const Math::Vector2& world_position);

    // Direct2D
    void DrawBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::Black, float stroke = 1.f);
    void DrawSolidBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::Black);
    void DrawGradientSolidBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& start_color, const Math::Color& end_color, GradientDirection direction = GradientDirection::kHorizontal);
    void DrawRoundBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::Black, float radius = 5.f, float stroke = 1.f);
    void DrawSolidRoundBox(const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::Black, float radius = 5.f);
    void DrawCircle(const Math::Vector2& position, float radius, const Math::Color& color = Math::Color::Black, float stroke = 1.f);
    void DrawSolidCircle(const Math::Vector2& position, float radius, const Math::Color& color = Math::Color::Black);
    void DrawString(const std::wstring& string, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::Black, const std::wstring& font_name = L"NanumBarunGothic", float font_size = 12.f, DWRITE_TEXT_ALIGNMENT text_alignment = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    void DrawLine(const Math::Vector2& start, const Math::Vector2& end, const Math::Color& color = Math::Color::Black, float stroke = 1.f);

    // 테스트
    void DrawStringWithOutline(const std::wstring& string, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& outline_color = Math::Color::Black, const Math::Color& fill_color = Math::Color::White, float stroke = 1.f, const std::wstring& font_name = L"NanumBarunGothic", float font_size = 12.f, DWRITE_TEXT_ALIGNMENT text_alignment = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    
    void DrawSimpleSprite(const UISprite* ui_sprite, uint64_t frame_index, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::White);
    void DrawSlicedSprite(const UISprite* ui_sprite, uint64_t frame_index, const Math::Vector2& position, const Math::Vector2& size, const Math::Color& color = Math::Color::White);
    
    bool LoadBitmap(const std::shared_ptr<WindowsWindow>& kWindow, const std::wstring& kFileName, Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap);

    Microsoft::WRL::ComPtr<IDWriteTextFormat> GetTextFormat(const std::wstring& kName, float size);

    bool GetTextAdvances(const std::wstring& kString, const std::wstring& kFontName, float font_size, const Math::Vector2& size, std::vector<float>& advances, std::vector<float>& line_heights);
    bool GetTextSize(const std::wstring& text, const std::wstring& font_name, float font_size, float& out_width, float& out_height);

    D2D1_RECT_F GetDip(const Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap, const D2D1_RECT_F& rect);

    FORCEINLINE ID3D11Device* GetDevice() const { return d3d_device_.Get(); }
    FORCEINLINE ID3D11DeviceContext* GetDeviceContext() const { return d3d_device_context_.Get(); }
    FORCEINLINE Microsoft::WRL::ComPtr<IWICImagingFactory> GetImageFactory() const { return wic_imaging_factory_; }

private:
    bool CreateBackBufferResources(Microsoft::WRL::ComPtr<IDXGISwapChain>& dxgi_swap_chain, Microsoft::WRL::ComPtr<ID3D11Texture2D>& back_buffer, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& d3d_render_target_view);

    Microsoft::WRL::ComPtr<ID3D11Device> d3d_device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d_device_context_;

    Microsoft::WRL::ComPtr<ID2D1Factory> d2d_factory_;

    Microsoft::WRL::ComPtr<IDWriteFactory5> dwrite_factory_;
    Microsoft::WRL::ComPtr<IDWriteFontCollection1> dwrite_font_collection_;
    
    Microsoft::WRL::ComPtr<IWICImagingFactory> wic_imaging_factory_;
    
    std::map<WindowsWindow*, Viewport> viewports_;
    std::map<WindowsWindow*, D2DViewport> d2d_viewports_;
    
    Viewport* current_viewport_;
    D2DViewport* current_d2d_viewport_;

    // TEST
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv_;

    Microsoft::WRL::ComPtr<ID3D11Buffer> post_vertex_buffer_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> post_index_buffer_;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> post_sampler_state_;
    
    std::shared_ptr<DefaultVertexShader> post_vertex_shader_;
    std::shared_ptr<DefaultPixelShader> post_pixel_shader_;
    
    uint32_t rtt_width_ = 0;
    uint32_t rtt_height_ = 0;

    Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> font_set_builder_;

    // Text Format
    std::unordered_map<std::wstring, std::unordered_map<float, Microsoft::WRL::ComPtr<IDWriteTextFormat>>> text_formats_;
    
};