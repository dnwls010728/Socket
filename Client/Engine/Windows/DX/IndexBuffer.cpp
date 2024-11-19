#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() :
    indices_(0),
    buffer_(nullptr)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::CreateBuffer(bool cpu_access, bool gpu_access)
{
    indices_ = 2048 * 3;
    Type::uint32 buffer_size = indices_ * sizeof(Type::uint32);

    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));

    buffer_desc.ByteWidth = buffer_size;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffer_desc.MiscFlags = 0;
    
    if (!cpu_access && gpu_access)
    {
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    }
    else if (!cpu_access && !gpu_access)
    {
        buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
    }
    else if (cpu_access && !gpu_access)
    {
        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
        buffer_desc.Usage = D3D11_USAGE_STAGING;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    }

    HRESULT hr = Renderer::Get()->GetDevice()->CreateBuffer(&buffer_desc, nullptr, buffer_.GetAddressOf());
    return SUCCEEDED(hr);
}

void* IndexBuffer::Lock()
{
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    Renderer::Get()->GetDeviceContext()->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    return mapped_resource.pData;
}

void IndexBuffer::Unlock()
{
    Renderer::Get()->GetDeviceContext()->Unmap(buffer_.Get(), 0);
}
