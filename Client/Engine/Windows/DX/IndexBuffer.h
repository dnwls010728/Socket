#pragma once
#include "Renderer.h"

#include <d3d11.h>
#include <wrl/client.h>

#include "Misc/Type.h"

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    bool CreateBuffer(bool cpu_access = false, bool gpu_access = true);

    void* Lock();

    void Unlock();
    
    FORCEINLINE ID3D11Buffer* GetResource() const { return buffer_.Get(); }

private:
    Type::uint32 indices_;
    
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    
};
