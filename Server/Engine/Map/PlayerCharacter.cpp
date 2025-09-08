#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>
#include <optional>
#include <ranges>
#include <unordered_set>

#include "DataManager.h"
#include "NetDef.h"
#include "StatEffect.h"
#include "StatEffectManager.h"
#include "World.h"
#include "Helper/StringHelper.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "MapObjects/DroppedItem.h"
#include "MySQL/MySQLManager.h"
#include "Session/PartyManager.h"
#include "Session/Party.h"
#include "Session/Player.h"
#include "Session/Player/Inventory/EquipItem.h"
#include "Session/Player/Inventory/Item.h"
#include "Skill/SkillManager.h"


PlayerCharacter::PlayerCharacter() :
    player_(),
    account_id_(0),
    party_id_(0),
    name_(L"Unknown"),
    body_color_(L"FFFFFF"),
    current_animation_(L"Idle"),
    map_id_(0),
    lv_(1),
    hp_(350),
    base_max_hp_(350),
    total_equip_stats_(),
    effective_max_hp_(0),
    effective_atk_(0),
    effective_def_(0),
    effective_dig_(0),
    is_dead_(false),
    is_flipped_(false),
    is_placing_(false),
    map_transitioning_(false),
    exp_(0),
    color_(0),
    inventories_(),
    buff_manager_(this),
    skill_manager_(this),
    is_invincible_(),
    dropped_item_mutex_(),
    effect_mutex_(),
    equip_stats_(),
    buff_effects_(),
    buff_expires_(),
    effects_(),
    buff_timer_(0.f)
{
    // 테스트
    skill_manager_.AddSkill(100000,1);
}

PlayerCharacter::~PlayerCharacter()
{
    if (map_) map_->RemovePlayer(object_id_);
}

void PlayerCharacter::SetPartyID(int32_t party_id)
{
    party_id_ = party_id;
}

std::shared_ptr<PlayerCharacter> PlayerCharacter::LoadCharacter(uint32_t character_id, const std::shared_ptr<Player>& player)
{
    std::shared_ptr<PlayerCharacter> character = std::make_shared<PlayerCharacter>();
    
    character->object_id_ = character_id;
    character->player_ = player;
    
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return nullptr;
    
    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM character_info WHERE character_id = ?"));
            statement->setUInt(1, character->object_id_);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                character->account_id_ = result->getInt("account_id");
                character->name_ = StringHelper::UTF8ToUTF16(result->getString("name"));
                character->body_color_ = StringHelper::UTF8ToUTF16(result->getString("body_color"));
                character->lv_ = result->getInt("lv");
                character->hp_ = result->getInt("hp");
                character->base_max_hp_ = result->getInt("max_hp");
                character->exp_.store(result->getInt("exp"));
                character->map_id_ = result->getInt("map_id");
                character->position_.x = static_cast<float>(result->getDouble("last_position_x"));
                character->position_.y = static_cast<float>(result->getDouble("last_position_y"));
                character->color_.store(result->getInt("color"));
            }
        }

        auto& inventories = character->inventories_;
        for (int32_t i = 1; i < static_cast<int32_t>(InventoryType::kCount); ++i)
        {
            inventories[i] = std::make_unique<Inventory>(character.get(), static_cast<InventoryType>(i));
            inventories[i]->SetCapacity(128);
        }

        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM inventory_item_info WHERE character_id = ?"));
            statement->setUInt(1, character->object_id_);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                uint32_t type_index = result->getUInt("inventory_type");
                uint32_t item_id = result->getInt("item_id");
                uint32_t slot_id = result->getInt("slot_id");
                
                int32_t count = result->getInt("count");

                if (type_index == 1 || type_index == 4)
                    inventories[type_index]->SetItem(slot_id, EquipItem::Create(item_id));
                else
                    inventories[type_index]->SetItem(slot_id, Item::Create(item_id, count));
            }
        }

        character->map_ = World::Get()->GetMap(character->map_id_);

        auto& equipped = character->inventories_[static_cast<uint8_t>(InventoryType::kEquipped)];
        for (const auto& it : equipped->GetItems())
        {
            auto item = std::dynamic_pointer_cast<EquipItem>(it.second);
            if (!item) continue;
            character->AddEquipStats(it.first, item);
        }

        character->ComputeStats();
        character->hp_ = std::min(character->hp_, character->effective_max_hp_);
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;

        return nullptr;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return nullptr;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return nullptr;
    }

    return character;
}

std::shared_ptr<PlayerCharacter> PlayerCharacter::CreateCharacter(const std::shared_ptr<Player>& player)
{
    std::shared_ptr<PlayerCharacter> character = std::make_shared<PlayerCharacter>();

    character->player_ = player;
    character->account_id_ = player->GetAccountID();
    
    auto& inventories = character->inventories_;
    for (int32_t i = 1; i < static_cast<int32_t>(InventoryType::kCount); ++i)
    {
        inventories[i] = std::make_unique<Inventory>(character.get(), static_cast<InventoryType>(i));
        inventories[i]->SetCapacity(128);
    }
    
    return character;
}

bool PlayerCharacter::DeleteCharacter(uint32_t character_id)
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return false;

    try
    {
        std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("DELETE FROM inventory_item_info WHERE character_id = ?"));
        statement->setUInt(1, character_id);
        statement->executeUpdate();
        
        statement.reset(connection->prepareStatement("DELETE FROM character_info WHERE character_id = ?"));
        statement->setUInt(1, character_id);
        statement->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        return false;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return false;
    }
    
    return true;
}

void PlayerCharacter::SendPacket(const Net::IPacket& packet) const
{
    if (auto player = player_.lock())
    {
        player->SendPacket(packet);
    }
}

void PlayerCharacter::ReceivePacket(Net::IPacket* packet)
{
    switch (packet->GetPacketID())
    {
    case ChangeMapPacket::StaticPacketID:
        {
            ChangeMapPacket* change_map_packet = static_cast<ChangeMapPacket*>(packet);
            if (map_transitioning_.load()) break;
            
            Portal* portal = map_->FindPortal(change_map_packet->portal_id);
            Map* to_map = World::Get()->GetMap(portal->GetToMap());
            if (!to_map) break;

            Portal* to_portal = to_map->FindPortal(portal->GetToID());
            if (!to_portal) break;

            is_placing_ = false;

            ChangeMap(to_map, to_portal);
        }
        break;
        
    case MapLoadCompletePacket::StaticPacketID:
        {
            map_->AddPlayer(std::static_pointer_cast<PlayerCharacter>(shared_from_this()));
            map_transitioning_.store(false);
        }
        break;

    case MovePlayerPacket::StaticPacketID:
        {
            MovePlayerPacket* move_player_packet = static_cast<MovePlayerPacket*>(packet);
            if (map_)
            {
                float position_x = move_player_packet->position_x;
                float position_y = move_player_packet->position_y;
                
                SetPosition({position_x, position_y});
                
                MovePlayerPacket move_player_broadcast_packet;
                move_player_broadcast_packet.unique_id = object_id_;
                move_player_broadcast_packet.position_x = position_x;
                move_player_broadcast_packet.position_y = position_y;
                move_player_broadcast_packet.velocity_x = move_player_packet->velocity_x;
                move_player_broadcast_packet.velocity_y = move_player_packet->velocity_y;
                move_player_broadcast_packet.server_time = Net::GetClientTime();
                move_player_broadcast_packet.time_update = move_player_packet->time_update;
                map_->SendPacket(move_player_broadcast_packet, std::static_pointer_cast<PlayerCharacter>(shared_from_this()));
            }
        }
        break;

    case PlayerAnimationPacket::StaticPacketID:
        {
            PlayerAnimationPacket* player_animation_packet = static_cast<PlayerAnimationPacket*>(packet);
            if (map_)
            {
                current_animation_ = player_animation_packet->animation;
                is_flipped_ = player_animation_packet->is_flipped;
                
                PlayerAnimationPacket player_animation_broadcast_packet;
                player_animation_broadcast_packet.unique_id = object_id_;
                player_animation_broadcast_packet.server_time = Net::GetClientTime();
                player_animation_broadcast_packet.animation = player_animation_packet->animation;
                player_animation_broadcast_packet.is_flipped =  player_animation_packet->is_flipped;
                map_->SendPacket(player_animation_broadcast_packet, std::static_pointer_cast<PlayerCharacter>(shared_from_this()));
            }
        }
        break;

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet);
            if (map_)
            {
                ChatMessagePacket chat_message_broadcast_packet;
                chat_message_broadcast_packet.unique_id = object_id_;
                chat_message_broadcast_packet.message = chat_message_packet->message;
                map_->SendPacket(chat_message_broadcast_packet);
            }
        }
        break;
        
    case MoveItemRequest::StaticPacketID:
        {
            MoveItemRequest* request = static_cast<MoveItemRequest*>(packet);

            auto& inventory = inventories_[request->inventory_type];
            {
                inventory->Lock();
                inventory->MoveOrStackSlots(request->first_slot, request->second_slot);
            }
        }
        break;

    case DropItemPacket::StaticPacketID:
        {
            DropItemPacket* request = static_cast<DropItemPacket*>(packet);

            uint8_t type_index = request->inventory_type;
            uint32_t slot_id = request->slot_id;

            const auto& inventory = inventories_[type_index];

            std::shared_ptr<Item> item = nullptr;

            InventoryUpdatePacket update_packet;
            {
                inventory->Lock();
                auto base_item = inventory->FindItem(slot_id);
                if (!base_item) break;

                int32_t count = base_item->GetCount();
                
                item = base_item->Clone();
                item->SetCount(request->count);
            
                if (request->count >= count)
                {
                    inventory->EraseItem(slot_id);

                    InventoryChange change;
                    change.inventory_type = type_index;
                    change.action = InventoryAction::kRemove;
                    change.remove.slot_id = slot_id;
                    update_packet.changes.push_back(change);
                }
                else
                {
                    int32_t remaining_count = count - request->count;
                    base_item->SetCount(remaining_count);

                    InventoryChange change;
                    change.inventory_type = type_index;
                    change.action = InventoryAction::kChangeCount;
                    change.change_count.slot_id = slot_id;
                    change.change_count.count = remaining_count;
                    update_packet.changes.push_back(change);
                }
            }
            SendPacket(update_packet);
            
            Math::Vector2 drop_position = GetPosition();
            map_->GetDropPosition(drop_position);
            
            map_->SpawnItemDrop(item, std::static_pointer_cast<PlayerCharacter>(shared_from_this()), drop_position);
        }
        break;

    case UseItemPacket::StaticPacketID:
        {
            UseItemPacket* use_item_packet = static_cast<UseItemPacket*>(packet);

            uint32_t slot_id = use_item_packet->slot_id;

            auto& inventory = inventories_[static_cast<uint8_t>(InventoryType::kUse)];

            std::shared_ptr<Item> item = nullptr;
            {
                inventory->Lock();
                item = inventory->FindItem(slot_id);
                
                if (!item && item->GetCount() <= 0) break;

                if (item->GetID() == 290000)
                {
                    if (is_placing_) break;
                    is_placing_ = true;
                    
                    PlacementStartPacket placement_start_packet;
                    SendPacket(placement_start_packet);
                }
                else
                {
                    item->SetCount(item->GetCount() - 1);

                    InventoryUpdatePacket inventory_update_packet;
                
                    if (item->GetCount() <= 0)
                    {
                        InventoryChange change;
                        change.inventory_type = static_cast<uint8_t>(InventoryType::kUse);
                        change.action = InventoryAction::kRemove;
                        change.remove.slot_id = slot_id;
                        inventory_update_packet.changes.push_back(change);
                    }
                    else
                    {
                        InventoryChange change;
                        change.inventory_type = static_cast<uint8_t>(InventoryType::kUse);
                        change.action = InventoryAction::kChangeCount;
                        change.change_count.slot_id = slot_id;
                        change.change_count.count = item->GetCount();
                        inventory_update_packet.changes.push_back(change);
                    }
                    
                    SendPacket(inventory_update_packet);
                }
                
                if (auto effect = StatEffectManager::Get()->FindItemEffect(item->GetID()))
                    effect->Apply(std::dynamic_pointer_cast<PlayerCharacter>(shared_from_this()));
            }
        }
        break;

    case PickupItemPacket::StaticPacketID:
        {
            PickupItemPacket* request = static_cast<PickupItemPacket*>(packet);

            std::shared_ptr<MapObject> map_object = map_->FindMapObject(request->object_id);
            if (!map_object) break;
            
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(map_object))
            {
                std::lock_guard<std::mutex> lock(dropped_item_mutex_);

                bool is_handled = false;
                if (dropped_item->IsColor())
                {
                    color_.fetch_add(dropped_item->GetColor());

                    ColorGainPacket color_gain_packet;
                    color_gain_packet.color = color_.load();
                    SendPacket(color_gain_packet);
                    
                    is_handled = true;
                }
                else
                {
                    auto item = dropped_item->GetItem();
                    
                    uint32_t type_index = item->GetID() / 100000;
                    auto& inventory = inventories_[type_index];
                    
                    {
                        inventory->Lock();
                        is_handled = inventory->AddItem(item);
                    }
                }
                
                if (is_handled)
                {
                    map_->DestroyDroppedItem(dropped_item->GetObjectID(), object_id_);
                }
            }
        }
        break;

    case EquipItemPacket::StaticPacketID:
        {
            EquipItemPacket* equip_item_packet = static_cast<EquipItemPacket*>(packet);

            uint32_t first_slot = equip_item_packet->first_slot;
            uint32_t second_slot = equip_item_packet->second_slot;

            auto& equip = inventories_[static_cast<uint8_t>(InventoryType::kEquip)];
            auto& equipped = inventories_[static_cast<uint8_t>(InventoryType::kEquipped)];

            InventoryUpdatePacket inventory_update_packet;
            std::shared_ptr<Item> first_item = nullptr;
            std::shared_ptr<Item> second_item = nullptr;
            
            {
                auto equip_lock = equip->DeferLock();
                auto equipped_lock = equipped->DeferLock();

                std::lock(equip_lock, equipped_lock);

                first_item = equip->EraseItem(first_slot);
                if (!first_item) break;

                // 착용한 아이템을 장비 탭에서 제거
                {
                    InventoryChange change;
                    change.inventory_type = static_cast<uint8_t>(InventoryType::kEquip);
                    change.action = InventoryAction::kRemove;
                    change.remove.slot_id = first_slot;
                    inventory_update_packet.changes.push_back(change);
                }

                second_item = equipped->EraseItem(second_slot);

                equipped->SetItem(second_slot, first_item);
                
                if (second_item)
                {
                    equip->SetItem(first_slot, second_item);

                    // 기존에 장착한 아이템을 장비창에서 제거
                    {
                        InventoryChange change;
                        change.inventory_type = static_cast<uint8_t>(InventoryType::kEquipped);
                        change.action = InventoryAction::kRemove;
                        change.remove.slot_id = second_item->GetSlot();
                        inventory_update_packet.changes.push_back(change);
                    }
                    
                    // 기존에 장착한 아이템을 장비 탭에 추가
                    {
                        InventoryChange change;
                        change.inventory_type = static_cast<uint8_t>(InventoryType::kEquip);
                        change.action = InventoryAction::kAdd;
                        change.add.slot_id = second_item->GetSlot();
                        change.add.item_id = second_item->GetID();
                        change.add.count = second_item->GetCount();
                        inventory_update_packet.changes.push_back(change);
                    }
                }
                
                // 장착한 아이템을 장비창에 추가
                {
                    InventoryChange change;
                    change.inventory_type = static_cast<uint8_t>(InventoryType::kEquipped);
                    change.action = InventoryAction::kAdd;
                    change.add.slot_id = first_item->GetSlot();
                    change.add.item_id = first_item->GetID();
                    change.add.count = first_item->GetCount();
                    inventory_update_packet.changes.push_back(change);
                }
            }
            SendPacket(inventory_update_packet);

            auto first_equip = std::dynamic_pointer_cast<EquipItem>(first_item);
            if (!first_equip) break;

            RemoveEquipStats(second_slot);
            AddEquipStats(second_slot, first_equip);
            
            ComputeStats();
            hp_ = std::min(hp_, effective_max_hp_);
            
            PlayerStatsUpdatePacket stats_update_packet;
            stats_update_packet.mask |= PlayerStat::kHP;
            stats_update_packet.mask |= PlayerStat::kMaxHP;
            stats_update_packet.hp = hp_;
            stats_update_packet.max_hp = effective_max_hp_;
            SendPacket(stats_update_packet);
        }
        break;

    case UnequipItemPacket::StaticPacketID:
        {
            UnequipItemPacket* equip_item_packet = static_cast<UnequipItemPacket*>(packet);
            
            uint32_t first_slot = equip_item_packet->first_slot;
            uint32_t second_slot = equip_item_packet->second_slot;
            if (second_slot == 0) break;
            
            auto& equip = inventories_[static_cast<uint8_t>(InventoryType::kEquip)];
            auto& equipped = inventories_[static_cast<uint8_t>(InventoryType::kEquipped)];

            InventoryUpdatePacket inventory_update_packet;
            {
                auto equip_lock = equip->DeferLock();
                auto equipped_lock = equipped->DeferLock();

                std::lock(equip_lock, equipped_lock);

                auto first_item = equipped->EraseItem(first_slot);
                if (!first_item) break;

                equip->SetItem(second_slot, first_item);

                // 장착한 아이템을 장비창에서 제거
                {
                    InventoryChange change;
                    change.inventory_type = static_cast<uint8_t>(InventoryType::kEquipped);
                    change.action = InventoryAction::kRemove;
                    change.remove.slot_id = first_slot;
                    inventory_update_packet.changes.push_back(change);
                }
                
                // 장착한 아이템을 장비 탭에 추가
                {
                    InventoryChange change;
                    change.inventory_type = static_cast<uint8_t>(InventoryType::kEquip);
                    change.action = InventoryAction::kAdd;
                    change.add.slot_id = first_item->GetSlot();
                    change.add.item_id = first_item->GetID();
                    change.add.count = first_item->GetCount();
                    inventory_update_packet.changes.push_back(change);
                }
            }
            SendPacket(inventory_update_packet);

            RemoveEquipStats(first_slot);
            ComputeStats();
            hp_ = std::min(hp_, effective_max_hp_);
            
            PlayerStatsUpdatePacket stats_update_packet;
            stats_update_packet.mask |= PlayerStat::kHP;
            stats_update_packet.mask |= PlayerStat::kMaxHP;
            stats_update_packet.hp = hp_;
            stats_update_packet.max_hp = effective_max_hp_;
            SendPacket(stats_update_packet);
        }
        break;

    case AttackRequest::StaticPacketID:
        {
            AttackRequest* attack_request = static_cast<AttackRequest*>(packet);
            if (!map_) return;

            map_->OnAttack(object_id_, attack_request->object_id);
        }
        break;

    case PlayerRespawnPacket::StaticPacketID:
        {
            hp_ = 50;
            
            PlayerStatsUpdatePacket stats_update_packet;
            stats_update_packet.mask |= PlayerStat::kHP;
            stats_update_packet.hp = hp_;
            SendPacket(stats_update_packet);

            NotifyPartyStatChange(PartyStatType::kHP, hp_);
            
            Respawn();
        }
        break;
        
    case PartyInviteRequest::StaticPacketID:
        {
            PartyInviteRequest* request = static_cast<PartyInviteRequest*>(packet);
            if (!map_) break;

            auto sendPopup = [this](const std::wstring &msg)
            {
                PopupPacket popup_packet;
                popup_packet.text = msg;
                SendPacket(popup_packet);
            };
            
            if (GetPartyID() == 0)
            {
                sendPopup(L"초대할 파티가 존재하지 않습니다.");
                break;
            }

            const auto party = PartyManager::Get()->GetParty(GetPartyID());
            if (party == nullptr)
            {
                sendPopup(L"초대할 파티가 존재하지 않습니다.");
                break;
            }
            
            auto invitee = map_->FindPlayer(request->invitee_id);
            if (!invitee)
            {
                sendPopup(L"대상을 찾을 수 없습니다.");
                break;
            }

            if (invitee->GetPartyID() == party->GetPartyID())
            {
                sendPopup(L"같은 파티에 포함되어 있는 플레이어 입니다.");
                break;
            }
            
            if (invitee->GetPartyID() != 0)
            {
                sendPopup(L"이미 가입되어 있는 파티가 존재하는 플레이어 입니다.");
                break;
            }

            PartyInviteNotify notify;
            notify.inviter_name = GetName();
            notify.party_name = party->GetPartyName();
            notify.party_id = party->GetPartyID();
            invitee->SendPacket(notify);
        }
        break;

    case PartyInviteNotifyResponse::StaticPacketID:
        {
            PartyInviteNotifyResponse* response = static_cast<PartyInviteNotifyResponse*>(packet);
            if (!map_) break;

            if (response->result == false)
            {
                auto inviter = map_->FindPlayer(response->inviter_id);
                if (inviter)
                {
                    PopupPacket popup_packet;
                    popup_packet.text = GetName() + L" 님이 파티 초대를 거절했습니다.";
                    inviter->SendPacket(popup_packet);
                }
                break;
            }

            auto party = PartyManager::Get()->GetParty(response->party_id);
            if (!party)
            {
                PopupPacket popup_packet;
                popup_packet.text = L"존재하지 않는 파티입니다.";
                SendPacket(popup_packet);
                break;
            }

            if (party->GetPlayerCount() > 10)
            {
                PopupPacket popup_packet;
                popup_packet.text = L"파티 인원이 꽉찼습니다.";
                SendPacket(popup_packet);
                break;
            }
            
            PartyManager::Get()->AddPlayerToParty(party->GetPartyID(), player_.lock());
            SetPartyID(party->GetPartyID());

            PopupPacket join_msg;
            join_msg.text = GetName() + L" 님이 파티에 합류했습니다.";
            PartyManager::Get()->SendPacket(party->GetPartyID(), join_msg, object_id_);
            
        }
        break;

    case PartyCreateRequest::StaticPacketID:
        {
            PartyCreateRequest* request = static_cast<PartyCreateRequest*>(packet);
            auto party = PartyManager::Get()->CreateParty(request->party_name);
            if (!party)
            {
                break;
            }

            PartyManager::Get()->AddPlayerToParty(party->GetPartyID(), player_.lock());
            SetPartyID(party->GetPartyID());

            PopupPacket popup_packet;
            popup_packet.text = L"파티가 정상적으로 생성되었습니다.";
            SendPacket(popup_packet);
        }
        break;

    case PartyKickRequest::StaticPacketID:
        {
            if (GetPartyID() == 0) break;
            auto* req = static_cast<PartyKickRequest*>(packet);
            auto party = PartyManager::Get()->GetParty(GetPartyID());
            if (!party) break;
            if (party->GetHost() != object_id_) break;
            if (req->member_id == object_id_) break;
            PartyManager::Get()->DeletePlayerFromParty(party->GetPartyID(), req->member_id);

            PopupPacket popup_packet;
            popup_packet.text = GetName() + L"파티에서 강퇴 되었습니다.";
            SendPacket(popup_packet);
        }
        break;

    case PartyDelegateRequest::StaticPacketID:
        {
            if (GetPartyID() == 0) break;
            auto* req = static_cast<PartyDelegateRequest*>(packet);
            auto party = PartyManager::Get()->GetParty(GetPartyID());
            if (!party) break;
            if (party->GetHost() != object_id_) break;
            party->DelegateHost(req->member_id);
        }
        break;

    case PartyLeavePacket::StaticPacketID:
        {
            if (GetPartyID() == 0) break;
            PartyManager::Get()->DeletePlayerFromParty(GetPartyID(), object_id_);
        }
        break;

    case PlacementStopRequest::StaticPacketID:
        {
            is_placing_ = false;

            PlacementStopResponse response;
            SendPacket(response);
        }
        break;

    case PlacementBlockPacket::StaticPacketID:
        {
            PlacementBlockPacket* placement_start_packet = static_cast<PlacementBlockPacket*>(packet);

            auto& inventory = inventories_[static_cast<uint8_t>(InventoryType::kUse)];
            
            {
                inventory->Lock();
                auto items = inventory->FindItems(290000);
                if (items.empty()) break;

                Math::Vector2 position;
                position.x = placement_start_packet->position.x;
                position.y = placement_start_packet->position.y;
            
                map_->SpawnBlock(L"FFFFFF", 0, position);

                auto item = items.front();
                item->SetCount(item->GetCount() - 1);

                InventoryUpdatePacket inventory_update_packet;
            
                if (item->GetCount() <= 0)
                {
                    InventoryChange change;
                    change.inventory_type = static_cast<uint8_t>(InventoryType::kUse);
                    change.action = InventoryAction::kRemove;
                    change.remove.slot_id = item->GetSlot();
                    inventory_update_packet.changes.push_back(change);
                
                    is_placing_ = false;

                    PlacementStopResponse placement_stop_response;
                    SendPacket(placement_stop_response);
                }
                else
                {
                    InventoryChange change;
                    change.inventory_type = static_cast<uint8_t>(InventoryType::kUse);
                    change.action = InventoryAction::kChangeCount;
                    change.change_count.slot_id = item->GetSlot();
                    change.change_count.count = item->GetCount();
                    inventory_update_packet.changes.push_back(change);
                }

                SendPacket(inventory_update_packet);
            }
        }
        break;

    case SkillCastRequest::StaticPacketID:
        {
            SkillCastRequest* skill_request = static_cast<SkillCastRequest*>(packet);
            skill_manager_.UseSkill(skill_request->skill_id);
        }
        break;

    default:
        break;
    }
}

void PlayerCharacter::TakeDamage(int32_t damage_amount)
{
    if (is_dead_ || is_invincible_) return;
    hp_ = std::clamp(hp_ - damage_amount, 0, effective_max_hp_);

    PlayerStatsUpdatePacket stats_update_packet;
    stats_update_packet.mask |= PlayerStat::kHP;
    stats_update_packet.hp = hp_;
    SendPacket(stats_update_packet);

    TakeDamagePacket packet;
    packet.object_id = object_id_;
    packet.damage_amount = damage_amount;
    packet.server_time = Net::GetClientTime();
    map_->SendPacket(packet);

    is_invincible_.Set(1.f);

    if (hp_ <= 0)
    {
        is_dead_ = true;
        
        PlayerDeathPacket death_packet;
        SendPacket(death_packet);
    }
    
    NotifyPartyStatChange(PartyStatType::kHP, hp_);
}

void PlayerCharacter::ApplyHPDelta(int32_t hp_delta)
{
    int32_t next_hp = hp_ + hp_delta;
    hp_ = std::clamp(next_hp, 1, effective_max_hp_);

    PlayerStatsUpdatePacket packet;
    packet.mask |= PlayerStat::kHP;
    packet.hp = hp_;
    SendPacket(packet);
    
    NotifyPartyStatChange(PartyStatType::kHP, hp_);
}

void PlayerCharacter::RegisterEffect(const std::shared_ptr<StatEffect>& effect, float start_time, float expire_time)
{
    {
        std::lock_guard<std::mutex> lock(effect_mutex_);

        int32_t effect_id = effect->GetBuffID();
        const auto& changes = effect->GetStatChanges();

        buff_expires_[effect_id] = expire_time;

        for (const auto& change : changes)
        {
            BuffStatValue candidate = { effect, start_time, change.second };
            buff_effects_[effect_id][change.first] = candidate;

            auto it = effects_.find(change.first);
            if (it == effects_.end() || IsBuffStronger(candidate, it->second))
                effects_[change.first] = std::move(candidate);
        }
    }

    ComputeStats();
}

bool PlayerCharacter::Disconnect()
{
    if (auto player = player_.lock())
    {
        player->Disconnect();
    }
    
    return false;
}

int32_t PlayerCharacter::GetBuffedValue(BuffStat stat) const
{
    auto it = effects_.find(stat);
    if (it != effects_.end())
        return it->second.value;
    
    return 0;
}

void PlayerCharacter::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);

    ObjectSpawnPacket packet;
    packet.object_info.type = ObjectType::kPlayer;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    PlayerInfo& info = packet.object_info.info.player;
    wcscpy_s(info.name, name_.c_str());
    wcscpy_s(info.body_color, body_color_.c_str());

    player->SendPacket(packet);

    PlayerAnimationPacket animation_packet;
    animation_packet.unique_id = object_id_;
    animation_packet.animation =  current_animation_;
    animation_packet.is_flipped = is_flipped_;
    animation_packet.server_time = Net::GetClientTime();
    player->SendPacket(animation_packet);
}

void PlayerCharacter::ChangeMap(Map* to, Portal* to_portal)
{
    map_transitioning_.store(true);

    map_->RemovePlayer(GetObjectID());
    map_ = to;
    
    SetPosition(to_portal->GetPosition() + Math::Vector2::Up());
    
    MapLoadPacket map_reset_packet;
    map_reset_packet.map_id = map_->GetMapID();
    map_reset_packet.spawn_position.x = GetPosition().x;
    map_reset_packet.spawn_position.y = GetPosition().y;
    SendPacket(map_reset_packet);
}

void PlayerCharacter::Respawn()
{
    Map* return_map = World::Get()->GetMap(map_->GetReturnMapID());
    if (!return_map) return;

    Portal* return_portal = return_map->FindPortal(0);
    if (!return_portal) return;

    is_dead_ = false;
    ChangeMap(return_map, return_portal);
}

void PlayerCharacter::ExitMap()
{
    if (map_)
    {
        map_->RemovePlayer(object_id_);
        map_ = nullptr;
    }
}

void PlayerCharacter::UpdateDatabase()
{
    uint32_t map_id = 0;
    if (map_) map_id = map_->GetMapID();
    
    if (is_dead_)
    {
        hp_ = 50;

        if (Map* return_map = World::Get()->GetMap(map_->GetReturnMapID()))
        {
            if (Portal* return_portal = return_map->FindPortal(0))
            {
                map_id = return_map->GetMapID();
                position_ = return_portal->GetPosition() + Math::Vector2::Up();
            }
        }
    }
    
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return;
    
    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("DELETE FROM inventory_item_info WHERE character_id = ?"));
            statement->setUInt(1, object_id_);
            statement->executeUpdate();

            statement.reset(connection->prepareStatement("INSERT INTO inventory_item_info (account_id, character_id, inventory_type, item_id, slot_id, count) VALUES (?, ?, ?, ?, ?, ?)"));
            for (int32_t i = 1; i < static_cast<int32_t>(InventoryType::kCount); ++i)
            {
                auto& inventory = inventories_[i];
                
                {
                    inventory->Lock();
                    for (const auto& slot : inventory->GetItems())
                    {
                        statement->setUInt(1, account_id_);
                        statement->setUInt(2, object_id_);
                        statement->setUInt(3, i);
                        statement->setUInt(4, slot.second->GetID());
                        statement->setUInt(5, slot.first);
                        statement->setInt(6, slot.second->GetCount());
                        statement->executeUpdate();
                    }
                }
            }
            
            statement.reset(connection->prepareStatement("UPDATE character_info SET hp = ?, max_hp = ?, exp = ?, lv = ?, map_id = ?, last_position_x = ?, last_position_y = ?, color = ? WHERE character_id = ?"));
            statement->setInt(1, hp_);
            statement->setInt(2, base_max_hp_);
            statement->setInt(3, exp_.load());
            statement->setInt(4, lv_);
            statement->setUInt(5, map_id);
            statement->setDouble(6, position_.x);
            statement->setDouble(7, position_.y);
            statement->setInt(8, color_.load());
            statement->setUInt(9, object_id_);
            statement->executeUpdate();
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
    }
}

void PlayerCharacter::GainExp(int32_t amount)
{
    if (lv_ >= 50) return;

    PlayerStatsUpdatePacket packet;

    bool changed_lv = false;
    
    int32_t new_exp = exp_.load();
    new_exp += amount;

    while (lv_ < 50)
    {
        int32_t need = DataManager::Get()->GetExp(lv_);
        
        if (new_exp < need) break;
        new_exp -= need;

        ++lv_;
        changed_lv = true;

        base_max_hp_ += 25;
        hp_ = base_max_hp_;

        ComputeStats();
        if (lv_ >= 50) break;
    }

    if (new_exp != exp_.load())
    {
        exp_.store(new_exp);
        packet.mask |= PlayerStat::kExp;
    }

    if (changed_lv)
    {
        packet.mask |= PlayerStat::kLv;
        packet.mask |= PlayerStat::kHP;
        packet.mask |= PlayerStat::kMaxHP;
    }
    
    if (EnumHasAnyFlags(packet.mask, PlayerStat::kHP)) packet.hp = hp_;
    if (EnumHasAnyFlags(packet.mask, PlayerStat::kMaxHP)) packet.max_hp = effective_max_hp_;
    if (EnumHasAnyFlags(packet.mask, PlayerStat::kExp)) packet.exp = exp_.load();
    if (EnumHasAnyFlags(packet.mask, PlayerStat::kLv)) packet.lv = lv_;

    SendPacket(packet);

    if (changed_lv)
    {
        NotifyPartyStatChange(PartyStatType::kLv, lv_);
        NotifyPartyStatChange(PartyStatType::kHP, hp_);
        NotifyPartyStatChange(PartyStatType::kMaxHP, effective_max_hp_);
    }
}

void PlayerCharacter::NotifyPartyStatChange(PartyStatType stat, int32_t value, bool exclude_self)
{
    if (party_id_ == 0)
        return;

    PartyMemberStatChangedPacket stat_packet;
    stat_packet.member_id = object_id_;
    stat_packet.stat = stat;
    stat_packet.value = std::to_wstring(value);

    if (exclude_self)
        PartyManager::Get()->SendPacket(party_id_, stat_packet, object_id_);
    else
        PartyManager::Get()->SendPacket(party_id_, stat_packet);
}

void PlayerCharacter::CheckBuffExpire()
{
    {
        std::lock_guard<std::mutex> lock(effect_mutex_);

        const float now = Net::GetClientTime();

        std::unordered_set<BuffStat> dirty;

        auto buff_expire_it = buff_expires_.begin();
        while (buff_expire_it != buff_expires_.end())
        {
            if (buff_expire_it->second > now)
            {
                ++buff_expire_it;
                continue;
            }
        
            const int32_t effect_id = buff_expire_it->first;
            buff_expire_it = buff_expires_.erase(buff_expire_it);

            auto buff_effect_it = buff_effects_.find(effect_id);
            if (buff_effect_it == buff_effects_.end()) continue;

            for (const auto& key : buff_effect_it->second | std::views::keys)
            {
                auto effect_it = effects_.find(key);
                if (effect_it != effects_.end() && effect_it->second.effect->GetID() == effect_id)
                {
                    effects_.erase(effect_it);
                    dirty.insert(key);
                }
            }

            buff_effects_.erase(buff_effect_it);
        }

        if (dirty.empty()) return;

        for (BuffStat stat : dirty)
        {
            const BuffStatValue* best = nullptr;

            for (const auto& val : buff_effects_ | std::views::values)
            {
                auto it = val.find(stat);
                if (it == val.end()) continue;

                if (!best || IsBuffStronger(it->second, *best))
                    best = &it->second;
            }

            if (best) effects_.insert_or_assign(stat, *best);
        }
    }

    ComputeStats();
}

void PlayerCharacter::ComputeStats()
{
    std::lock_guard<std::mutex> lock(effect_mutex_);

    effective_max_hp_ = base_max_hp_ + total_equip_stats_.max_hp;
    effective_atk_ = total_equip_stats_.atk + GetBuffedValue(BuffStat::kAtk);
    effective_def_ = total_equip_stats_.def + GetBuffedValue(BuffStat::kDef);
    effective_dig_ = total_equip_stats_.dig + GetBuffedValue(BuffStat::kDig);
}

void PlayerCharacter::AddEquipStats(uint32_t slot, const std::shared_ptr<EquipItem>& item)
{
    if (!item) return;

    EquipStat stat = { item->GetMaxHP(), item->GetAtk(), item->GetDef(), item->GetDig() };
    equip_stats_.insert_or_assign(slot, stat);

    total_equip_stats_ += stat;
}

void PlayerCharacter::RemoveEquipStats(uint32_t slot)
{
    auto it = equip_stats_.find(slot);
    if (it == equip_stats_.end()) return;

    total_equip_stats_ -= it->second;
    equip_stats_.erase(it);
}

bool PlayerCharacter::IsBuffStronger(const BuffStatValue& new_effect, const BuffStatValue& existing_effect) const
{
    if (new_effect.value != existing_effect.value)
        return new_effect.value > existing_effect.value;

    uint64_t existing_size = existing_effect.effect->GetStatChanges().size();
    uint64_t new_size = new_effect.effect->GetStatChanges().size();
    return new_size > existing_size;
}

void PlayerCharacter::Tick(float delta_time)
{
    MapObject::Tick(delta_time);
    
    buff_manager_.Tick(delta_time);
    skill_manager_.Tick(delta_time);

    is_invincible_.Tick(delta_time);
    
}
