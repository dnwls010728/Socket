#include "pch.h"
#include "Player.h"

#include <CustomPacket.h>

#include "DataManager.h"
#include "IPacket.h"
#include "Session.h"
#include "../Map/World.h"
#include "../MySQL/MySQLManager.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "Player/Inventory/Inventory.h"

Player::Player(Session* session, uint32_t account_id) :
    session_(session),
    account_id_(account_id),
    character_info_(),
    player_character_(nullptr)
{
}

Player::~Player()
{
}

void Player::SendPacket(const Net::IPacket& packet) const
{
    if (!session_) return;
    session_->SendPacket(packet);
}

void Player::ReceivePacket(Net::IPacket* packet)
{
    switch (packet->GetPacketID())
    {
    case SelectCharacterRequest::StaticPacketID:
        {
            SelectCharacterRequest* request = static_cast<SelectCharacterRequest*>(packet);
            player_character_ = PlayerCharacter::LoadCharacter(request->unique_id, shared_from_this());
            
            SelectCharacterResponse response;
            response.is_success = true;
            response.message = L"";
            response.name = player_character_->name_;
            response.character_id = player_character_->object_id_;
            response.lv = player_character_->lv_;
            response.hp = player_character_->hp_;
            response.max_hp = player_character_->max_hp_;
            response.exp = player_character_->exp_;
            response.color = player_character_->color_;
            response.position_x = player_character_->position_.x;
            response.position_y = player_character_->position_.y;

            for (const auto& it : player_character_->inventory_->GetSlots())
            {
                ItemInfo item_info;
                item_info.item_id = it.second.item_id;
                item_info.slot_index = it.first;
                item_info.count = it.second.count;

                response.inventory.push_back(item_info);
            }

            SendPacket(response);

            session_->SetState(Session::State::kCharacterSelected);
        }
        break;
        
    case InGameReadyPacket::StaticPacketID:
        {
            session_->SetState(Session::State::kInGame);
        }
        break;
        
    default:
        break;
    }

    if (player_character_) player_character_->ReceivePacket(packet);
}

void Player::Update()
{
    if (player_character_) player_character_->UpdateCharacter();
}

void Player::ExitMap()
{
    if (player_character_)
    {
        player_character_->ExitMap();
        player_character_ = nullptr;
    }
}
