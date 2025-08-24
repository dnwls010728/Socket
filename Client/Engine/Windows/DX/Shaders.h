#pragma once
#include "Windows/DX/Renderer.h"

#include <string>
#include <wrl/client.h>

#include "ConstantBuffer.h"
#include "DirectXTK/VertexTypes.h"

class VertexShader
{
public:
    VertexShader();
    virtual ~VertexShader() = default;

    bool Create(const std::wstring& kPath, const D3D11_INPUT_ELEMENT_DESC* kLayout, UINT layout_count);

    void BindShader();
    void BindParameters();

protected:
    virtual void UpdateParameters() {}

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader_;
    Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer_;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_;
};

class PixelShader
{
public:
    PixelShader();
    virtual ~PixelShader() = default;

    bool Create(const std::wstring& kPath);

    void BindShader();
    void BindParameters();
    
protected:
    virtual void UpdateParameters() {}

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
    Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer_;
};

class DefaultVertexShader : public VertexShader
{
public:
    DefaultVertexShader();
    virtual ~DefaultVertexShader() override = default;

    void SetWorldMatrix(const DirectX::XMMATRIX& kMat);
    void SetUVOffset(const DirectX::XMFLOAT2& kOffset);
    void SetUVScale(const DirectX::XMFLOAT2& kScale);

protected:
    virtual void UpdateParameters() override;

private:
    struct Constants
    {
        DirectX::XMMATRIX mat;
        DirectX::XMFLOAT2 uv_offset = {0, 0};
        DirectX::XMFLOAT2 uv_scale = {1, 1};
    };

    ConstantBuffer<Constants> constant_buffer_;
};

class DefaultPixelShader : public PixelShader
{
public:
    DefaultPixelShader();
    virtual ~DefaultPixelShader() override = default;

    void EnableTexture(BOOL has_texture);
    void SetColor(const Math::Color& kColor);

protected:
    virtual void UpdateParameters() override;

private:
    struct Constants
    {
        // 16 bytes
        float r = 1.f;
        float g = 1.f;
        float b = 1.f;
        float a = 1.f;

        // 16 bytes
        BOOL has_texture = FALSE;
        BOOL padding[3];
    };

    ConstantBuffer<Constants> constant_buffer_;
};

class PostProcessShader : public PixelShader
{
public:
    PostProcessShader();
    virtual ~PostProcessShader() override = default;

    void SetResolution(const DirectX::XMFLOAT2& res);
    void SetBlurRadius(float radius);
    void SetVignette(float strength);
    void SetGamma(float gamma);
    void SetGrayscale(float grayscale);

protected:
    virtual void UpdateParameters() override;

private:
    struct Constants
    {
        DirectX::XMFLOAT2 resolution;
        float blur_radius = 1.0f;
        float vignette_strength = 0.0f;
        float gamma = 1.0f;
        float grayscale = 0.f;
        float padding[2];
    };

    ConstantBuffer<Constants> constant_buffer_;
};
