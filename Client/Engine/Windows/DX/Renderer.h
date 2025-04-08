#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d2d1.lib")
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
#include "Math/Rect.h"
#include "Misc/Type.h"
#include "Windows/WindowsWindow.h"

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
    bool ResizeViewport(const std::shared_ptr<WindowsWindow>& kWindow, Type::uint32 width, Type::uint32 height);
    bool AddFont(const std::wstring& kPath);
    bool AddTextFormat(const std::wstring& kName, float size);
    
    // TEST
    bool CreateRenderToTexture();

    void BeginRTT();
    void EndRTT();

    FORCEINLINE const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV() const { return srv_; }

    Viewport* FindViewport(WindowsWindow* window);
    D2DViewport* FindD2DViewport(WindowsWindow* window);

    void BeginRender(const std::shared_ptr<WindowsWindow>& kWindow);
    void EndRender();
    void BeginRenderD2D(const std::shared_ptr<WindowsWindow>& kWindow);
    void EndRenderD2D();
    void BeginLayer(const Math::Rect& kRect);
    void EndLayer();
    void ChangeResolution(WindowsWindow* window, Type::uint32 width, Type::uint32 height, bool is_fullscreen = false);
    
    Math::Vector2 ConvertScreenToWorld(const Math::Vector2& kScreenPosition) const;
    Math::Vector2 ConvertWorldToScreen(const Math::Vector2& kWorldPosition) const;

    // Direct2D
    void DrawBox(WindowsWindow* window, const Math::Rect& kRect, const Math::Vector2& kPivot, const Math::Color& kColor, float angle = 0.f, float stroke = 1.f);
    void DrawSolidBox(WindowsWindow* window, const Math::Rect& kRect, const Math::Vector2& kPivot, const Math::Color& kColor, float angle = 0.f);
    void DrawRoundBox(WindowsWindow* window, const Math::Rect& kRect, const Math::Vector2& kPivot, const Math::Color& kColor, float radius, float angle = 0.f, float stroke = 1.f);
    void DrawSolidRoundBox(WindowsWindow* window, const Math::Rect& kRect, const Math::Vector2& kPivot, const Math::Color& kColor, float radius, float angle = 0.f);
    void DrawCircle(const std::shared_ptr<WindowsWindow>& kWindow, Math::Vector2 position, float radius, Math::Color color, float stroke = 1.f);
    void DrawSolidCircle(const std::shared_ptr<WindowsWindow>& kWindow, Math::Vector2 position, float radius, Math::Color color);
    void DrawLine(WindowsWindow* window, Math::Vector2 start, Math::Vector2 end, Math::Color color, float stroke = 1.f);
    void DrawString(WindowsWindow* window, const std::wstring& kString, const Math::Rect& kRect, const Math::Vector2& kPivot, const Math::Color& kColor, float angle = 0.f, const std::wstring& kFontName = L"", float font_size = 0.f, DWRITE_TEXT_ALIGNMENT text_alignment = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    void DrawBitmap(WindowsWindow* window, const Microsoft::WRL::ComPtr<ID2D1Bitmap>& kBitmap, const Math::Rect& kRect, const Math::Vector2& kPivot, float angle = 0.f, bool use_slice9 = false, const Math::Rect& kSlice9Rect = Math::Rect::Zero());
    
    bool LoadBitmap(const std::shared_ptr<WindowsWindow>& kWindow, const std::wstring& kFileName, Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap);

    Microsoft::WRL::ComPtr<IDWriteTextFormat> GetTextFormat(const std::wstring& kName, float size);

    bool GetTextAdvances(/*const Math::Rect& kRect, */const std::wstring& kString, const std::wstring& kFontName, float font_size, std::vector<float>& advances);

    FORCEINLINE ID3D11Device* GetDevice() const { return d3d_device_.Get(); }
    FORCEINLINE ID3D11DeviceContext* GetDeviceContext() const { return d3d_device_context_.Get(); }
    FORCEINLINE Microsoft::WRL::ComPtr<IWICImagingFactory> GetImageFactory() const { return wic_imaging_factory_; }

private:
    bool CreateBackBufferResources(Microsoft::WRL::ComPtr<IDXGISwapChain>& dxgi_swap_chain, Microsoft::WRL::ComPtr<ID3D11Texture2D>& back_buffer, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& d3d_render_target_view);

    void DrawSlice(D2DViewport* d2d_viewport, const Microsoft::WRL::ComPtr<ID2D1Bitmap>& kBitmap, const D2D1_RECT_F& kSrcRect, const D2D1_RECT_F& kDestRect);

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

    Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> font_set_builder_;

    // Text Format
    std::unordered_map<std::wstring, std::unordered_map<float, Microsoft::WRL::ComPtr<IDWriteTextFormat>>> text_formats_;
    
};