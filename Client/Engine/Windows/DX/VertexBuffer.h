#pragma once
#include "Renderer.h"

#include <d3d11.h>
#include <wrl/client.h>

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    bool CreateBuffer(uint32_t stride, bool cpu_access = false, bool gpu_access = true);

    void* Lock();
    
    void Unlock();

    FORCEINLINE uint32_t GetBufferSize() const { return buffer_size_; }
    
    FORCEINLINE ID3D11Buffer* GetResource() const { return buffer_.Get(); }

private:
    uint32_t stride_;
    uint32_t buffer_size_;

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    
};
