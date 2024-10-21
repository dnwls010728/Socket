#pragma once
#include "Renderer.h"

#include <d3d11.h>
#include <wrl/client.h>

#include "Misc/Type.h"

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    bool CreateBuffer(Type::uint32 stride, bool cpu_access = false, bool gpu_access = true);

    void* Lock();
    
    void Unlock();

    inline Type::uint32 GetBufferSize() const { return buffer_size_; }
    
    inline ID3D11Buffer* GetResource() const { return buffer_.Get(); }

private:
    Type::uint32 stride_;
    Type::uint32 buffer_size_;

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    
};
