#pragma once

class Session
{
public:
    Session(int client_id);
    ~Session() = default;

    inline int GetClientID() const { return client_id_; }

    inline void SetAccountUniqueID(int account_unique_id) { account_unique_id_ = account_unique_id; }
    inline int GetAccountUniqueID() const { return account_unique_id_; }

    inline void SetCharacterUniqueID(int character_unique_id) { character_unique_id_ = character_unique_id; }
    inline int GetCharacterUniqueID() const { return character_unique_id_; }

private:
    int client_id_;
    int account_unique_id_;
    int character_unique_id_;
    
};
