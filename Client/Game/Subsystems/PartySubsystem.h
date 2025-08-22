#pragma once
#include "Subsystem/GameInstanceSubsystem.h"
#include <CommonObject.h>
#include <unordered_map>

class PartySubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(PartySubsystem, GameInstanceSubsystem)

public:
    PartySubsystem();
    virtual ~PartySubsystem() override = default;

    void AddOrUpdateMember(const PartyMemberInfo& info);
    void DeleteMember(uint32_t id);
    void UpdateMemberStat(uint32_t id, PartyStatType stat, const std::wstring& value);
    void Clear();
    
    FORCEINLINE bool IsJoinedParty() const { return party_id_ != 0; }
    
    FORCEINLINE uint32_t GetPartyID() const { return party_id_; }
    FORCEINLINE void SetPartyID(uint32_t party_id) { party_id_ = party_id; }

    FORCEINLINE std::wstring GetPartyName() const { return party_name_; }
    FORCEINLINE void SetPartyName(const std::wstring& name) { party_name_ = name; }

    FORCEINLINE uint32_t GetHostMemberID() const { return host_member_id_; }
    FORCEINLINE void SetHostMemberID(uint32_t host_member_id) {  host_member_id_ = host_member_id; }
    
    
    const std::unordered_map<uint32_t, PartyMemberInfo>& GetMembers() const { return members_; }

    static PartySubsystem* Get();

private:
    void UpdateUIAddOrUpdate(const PartyMemberInfo& info);
    void UpdateUIRemove(uint32_t id);

    std::unordered_map<uint32_t, PartyMemberInfo> members_;
    uint32_t party_id_;
    std::wstring party_name_;
    uint32_t host_member_id_;
};
