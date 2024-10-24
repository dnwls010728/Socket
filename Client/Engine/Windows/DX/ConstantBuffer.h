#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Renderer.h"

template <typename T>
class ConstantBuffer
{
public:
    bool Create()
    {
        D3D11_BUFFER_DESC buffer_desc;
        ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));

        buffer_desc.ByteWidth = sizeof(T);
        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;

        HRESULT hr = Renderer::Get()->GetDevice()->CreateBuffer(&buffer_desc, nullptr, buffer_.GetAddressOf());
        return SUCCEEDED(hr);
    }

    FORCEINLINE ID3D11Buffer* GetResource() const { return buffer_.Get(); }
    FORCEINLINE ID3D11Buffer* const* GetResourceAddress() const { return buffer_.GetAddressOf(); }
    
    FORCEINLINE T& GetBufferData() { return buffer_data_; }

    void UpdateBuffer()
    {
        void* data = Lock();
        CopyMemory(data, &buffer_data_, sizeof(T));
        Unlock();
    }

private:
    void* Lock()
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        Renderer::Get()->GetDeviceContext()->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        return mapped_resource.pData;
    }

    void Unlock()
    {
        Renderer::Get()->GetDeviceContext()->Unmap(buffer_.Get(), 0);
    }
    
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;

    T buffer_data_;
    
};
