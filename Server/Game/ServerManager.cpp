#include "pch.h"
#include "ServerManager.h"
#include <string>
#include <iostream>
#include "NetworkManager.h"
#include "CustomPacket.h"
#include "DataManager.h"
#include "Session/Session.h"
#include "Helper/StringHelper.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "Map/MapObjects/Mob/Mob.h"
#include "Session/SessionManager.h"

ServerManager::ServerManager()
{
    // 콜백 설정
    server_socket_.SetClientAcceptedCallback(std::bind(&ServerManager::OnClientConnected, this, std::placeholders::_1));
    server_socket_.SetClientDisconnectedCallback(std::bind(&ServerManager::OnClientDisconnected, this, std::placeholders::_1));
    server_socket_.SetPacketReceivedCallback(std::bind(&ServerManager::OnPacketReceived, this, std::placeholders::_1, std::placeholders::_2));

    CommandHandlerInitialize();
}

void ServerManager::CommandHandlerInitialize()
{
    command_handler_[L"/disconnect"] = [&](auto& args) {
        if (args.size() < 2) {
            std::wcout << L"Usage: /disconnect <clientKey>\n";
            return;
        }
        try {
            uint32_t key = std::stoul(args[1]);
            if (!server_socket_.DisconnectClient(key))
                std::wcout << L"Error: No client with key " << key << "\n";
            else
            {
                std::wcout << L"Client " << key << " disconnected\n";
            }
        }
        catch (...) {
            std::wcout << L"Error: Invalid key '" << args[1] << "'\n";
        }
    };

    command_handler_[L"/help"] = [&](auto&) {
        std::wcout << L"Available commands:\n";
        std::wcout << L"  /exit                   - Exit the server\n";
        std::wcout << L"  /disconnect <clientKey> - Disconnect a client by its key\n";
        std::wcout << L"  /help                   - Show this help\n";
    };
}

bool ServerManager::Execute()
{
    DataManager::Get()->Init();
    
    MySQLManager* mysql_manager = MySQLManager::Get();
    // if (!mysql_manager->Connect("localhost", "root", "12345"))

#ifdef _DEBUG
    if (!mysql_manager->Connect("58.79.118.105", "y_eternal", "@eternal12345"))
#else
    if (!mysql_manager->Connect("localhost", "y_eternal", "@eternal12345"))
#endif
    {
        return false;
    }

    Net::WSAInit();

    Net::NetAddress server_address("0.0.0.0", 9101);
    if (server_socket_.Start(server_address, 0) == false)
    {
        return false;
    }

     World::Get()->Start(100, 10);
    
    // 명령어 입력 루프
    std::wstring line;
    while (true)
    {
        std::wcout << L"> ";
        if (!std::getline(std::wcin, line))
            break;
        
        std::wistringstream wiss(line);
        std::vector<std::wstring> tokens;
        std::wstring tok;
        while (wiss >> tok) tokens.push_back(tok);
        
        if (tokens.empty()) continue;

        // 종료
        if (tokens[0] == L"/exit")
        {
            break;
        }
        
        auto it = command_handler_.find(tokens[0]);
        if (it != command_handler_.end()) {
            it->second(tokens);
        }
        else {
            std::cout << "Unrecognized command. Type /help for list.\n";
        }
    }

    World::Get()->Stop();
    
    server_socket_.Stop();
    Net::WSAUninit();
    mysql_manager->Disconnect();
    return true;
}

bool ServerManager::OnClientConnected(const Net::TCPConnectionState& state)
{
    std::cout << "Client connected: " << state.address.ToString().c_str() << std::endl;
    
    std::unique_ptr<Session> session = std::make_unique<Session>(state.uniqueKey);
    session->SetState(Session::State::kConnected);
    
    SessionManager::Get()->AddSession(session);
    return true;
}

bool ServerManager::DisconnectClient(int client_id)
{
    return server_socket_.DisconnectClient(client_id);
}

void ServerManager::OnClientDisconnected(const Net::TCPConnectionState& state)
{
    std::cout << "Client disconnected: " << state.address.ToString().c_str() << std::endl;

    // 유저 데이터 저장
    SessionManager* session_manager = SessionManager::Get();
    
    Session* session = session_manager->FindSessionByClientID(state.uniqueKey);
    session->Update();
    
    session_manager->RemoveSession(state.uniqueKey);
}

void ServerManager::OnPacketReceived(const Net::TCPConnectionState& state, std::unique_ptr<Net::IPacket> packet)
{
    SessionManager* session_manager = SessionManager::Get();
    
    {
        Session* session = session_manager->FindSessionByClientID(state.uniqueKey);
        if (session) session->ReceivePacket(packet.get());
    }
    
    switch (packet->GetPacketID())
    {
    case MessagePacket::StaticPacketID:
        {
            MessagePacket* message_packet = static_cast<MessagePacket*>(packet.get());
            std::cout << "Message received from " << state.address.ToString().c_str() << ": " << message_packet->message.c_str() << std::endl;
        }
        break;

    case DisconnectPacket::StaticPacketID:
        {
            DisconnectPacket* disconnect_packet = static_cast<DisconnectPacket*>(packet.get());
            
        }
        break;

    case LoginRequest::StaticPacketID:
        {
            LoginRequest* request = static_cast<LoginRequest*>(packet.get());
            
            sql::Connection* connection = MySQLManager::Get()->GetConnection();
            if (!connection) break;

            try
            {
                std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM account_info WHERE id = ? AND password = ?"));
                statement->setString(1, StringHelper::UTF16ToUTF8(request->id));
                statement->setString(2, StringHelper::UTF16ToUTF8(request->password));

                std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
                if (result->next())
                {
                    int account_id = result->getInt("account_id");
                    if (session_manager->HasSessionByAccountID(account_id))
                    {
                        LoginResponse response;
                        response.is_success = false;
                        response.message = L"이미 로그인된 계정입니다.";
                        server_socket_.SendPacketToClient(state.uniqueKey, response);
                        break;
                    }

                    Session* session = session_manager->FindSessionByClientID(state.uniqueKey);
                    if (!session)
                    {
                        LoginResponse response;
                        response.is_success = false;
                        response.message = L"세션을 찾을 수 없습니다.";
                        server_socket_.SendPacketToClient(state.uniqueKey, response);
                        break;
                    }
                    
                    session->CreatePlayer(account_id);

                    std::vector<CharacterProfile> profiles;
                    for (const auto& character : session->GetPlayer()->GetCharacters())
                    {
                        CharacterProfile profile;
                        profile.character_id = character->GetObjectID();
                        profile.map_id = character->GetMap()->GetMapID();
                        profile.name = character->GetName();
                        
                        profile.position.x = character->GetPosition().x;
                        profile.position.y = character->GetPosition().y;

                        profile.body_color = character->GetBodyColor();

                        profile.stats[static_cast<uint8_t>(PlayerStat::kHP)] = character->hp_;
                        profile.stats[static_cast<uint8_t>(PlayerStat::kMaxHP)] = character->max_hp_;
                        profile.stats[static_cast<uint8_t>(PlayerStat::kExp)] = character->exp_;
                        profile.stats[static_cast<uint8_t>(PlayerStat::kLv)] = character->lv_;
                        
                        profiles.push_back(profile);
                    }
                    
                    LoginResponse response;
                    response.is_success = true;
                    response.message = L"";
                    response.profiles = profiles;
                    server_socket_.SendPacketToClient(state.uniqueKey, response);

                    session->SetState(Session::State::kLoggedIn);
                }
                else
                {
                    LoginResponse response;
                    response.is_success = false;
                    response.message = L"아이디 또는 비밀번호가 잘못되었습니다.";
                    server_socket_.SendPacketToClient(state.uniqueKey, response);
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
        break;

    default:
        break;
    }
}

void ServerManager::SendPacket(uint32_t client_id, const Net::IPacket& packet)
{
    server_socket_.SendPacketToClient(client_id, packet);
}
