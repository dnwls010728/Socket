#include "pch.h"
#include "ChannelGroup.h"

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::enumeration<ChannelGroup>("ChannelGroup")
        (
            value("BGM", ChannelGroup::kBGM),
            value("SE", ChannelGroup::kSE),
            value("MobSE", ChannelGroup::kMobSE),
            value("SkillSE", ChannelGroup::kSkillSE)
        );
}
