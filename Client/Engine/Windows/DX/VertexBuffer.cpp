#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() :
    stride_(0),
    buffer_size_(0),
    buffer_(nullptr)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::CreateBuffer(Type::uint32 stride, bool cpu_access, bool gpu_access)
{
    stride_ = stride;
    buffer_size_ = stride * 2048;

    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));

    buffer_desc.ByteWidth = buffer_size_;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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

void* VertexBuffer::Lock()
{
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    Renderer::Get()->GetDeviceContext()->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    return mapped_resource.pData;
}

void VertexBuffer::Unlock()
{
    Renderer::Get()->GetDeviceContext()->Unmap(buffer_.Get(), 0);
}
