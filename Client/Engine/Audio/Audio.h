#pragma once
#include "FMOD/fmod_common.h"
#include "Asset/Asset.h"

struct AudioFormat
{
    FMOD_SOUND_TYPE type;
    FMOD_SOUND_FORMAT format;
    int channels;
    int bits;
};

class Audio : public Asset
{
    GENERATED_BODY(Audio, Asset)
    
public:
    Audio();
    virtual ~Audio() override;

    virtual bool Load(const std::wstring& kPath) override;

    void SetLoop(bool is_loop);

    Type::uint32 GetLength() const;

    AudioFormat GetFormat() const;

private:
    friend class AudioManager;
    
    FMOD_SOUND* sound_;
    
};
