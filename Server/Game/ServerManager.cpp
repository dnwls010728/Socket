#include "pch.h"
#include "ServerManager.h"
#include <string>
#include <iostream>
#include "NetworkManager.h"
#include "CustomPacket.h"
#include "EngineSettings.h"
#include "Session/Session.h"
#include "Helper/StringHelper.h"

ServerManager::ServerManager()
{
    // 콜벡 설정file:/D:/Users/Desktop/태양/게임엔진/Socket/Server/ServerManager.h
    server_socket_.SetClientAcceptedCallback(std::bind(&ServerManager::OnClientConnected, this, std::placeholders::_1));
    server_socket_.SetClientDisconnectedCallback(std::bind(&ServerManager::OnClientDisconnected, this, std::placeholders::_1));
    server_socket_.SetPacketReceivedCallback(std::bind(&ServerManager::OnPacketReceived, this, std::placeholders::_1, std::placeholders::_2));

    CommandHandlerInitialize();

    EngineSettings::Get()->SetFixedTimeStep(0.05f);
    EngineSettings::Get()->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault | ActorLayer::kCharacter);
    EngineSettings::Get()->AddCollisionLayer(ActorLayer::kCharacter, ActorLayer::kDefault);
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
    MySQLManager* mysql_manager = MySQLManager::Get();
    // if (!mysql_manager->Connect("poroserver.iptime.org", "y_eternal", "@eternal12345"))
    if (!mysql_manager->Connect("127.0.0.1:3306", "y_eternal", "@eternal12345"))
    {
        return false;
    }

    Net::WSAInit();

    Net::NetAddress server_address("0.0.0.0", 9000);
    if (server_socket_.Start(server_address, 0) == false)
    {
        return false;
    }

    World::Get()->Start(50,10);
    
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
    
    session_manager_.AddSession(session);
    return true;
}

void ServerManager::OnClientDisconnected(const Net::TCPConnectionState& state)
{
    std::cout << "Client disconnected: " << state.address.ToString().c_str() << std::endl;
    session_manager_.RemoveSession(state.uniqueKey);
}

void ServerManager::OnPacketReceived(const Net::TCPConnectionState& state, std::unique_ptr<Net::IPacket> packet)
{
    Session* session = session_manager_.FindSessionByClientID(state.uniqueKey);
    if (session) session->ReceivePacket(packet.get());
    
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

    case RegisterRequest::StaticPacketID:
        {
            RegisterRequest* request = static_cast<RegisterRequest*>(packet.get());
            MySQLManager* mysql_manager = MySQLManager::Get();
            
            bool is_found = false;
            mysql_manager->ExecuteQuery(L"SELECT * FROM account_info WHERE id = '" + request->id + L"'", [&](const sql::ResultSet* result)
            {
                is_found = true;

                RegisterResponse response;
                response.is_success = false;
                response.message = L"ID already exists.";
                server_socket_.SendPacketToClient(state.uniqueKey, response);
            });

            if (!is_found)
            {
                int result = mysql_manager->ExecuteUpdate(L"INSERT INTO account_info (id, password) VALUES ('" + request->id + L"', '" + request->password + L"')");
                if (result == 0)
                {
                    RegisterResponse response;
                    response.is_success = false;
                    response.message = L"Registration failed.";
                    server_socket_.SendPacketToClient(state.uniqueKey, response);
                    break;
                }
                
                RegisterResponse response;
                response.is_success = true;
                response.message = L"Registration successful.";
                server_socket_.SendPacketToClient(state.uniqueKey, response);
            }
        }
        break;

    case LoginRequest::StaticPacketID:
        {
            LoginRequest* request = static_cast<LoginRequest*>(packet.get());
            MySQLManager* mysql_manager = MySQLManager::Get();

            bool is_found = false;
            mysql_manager->ExecuteQuery(L"SELECT * FROM account_info WHERE id = '" + request->id + L"' AND password = '" + request->password + L"'", [&](const sql::ResultSet* result)
            {
                is_found = true;

                int unique_id = result->getInt("unique_id");
                if (session_manager_.HasSessionByAccountUniqueID(unique_id))
                {
                    LoginResponse response;
                    response.is_success = false;
                    response.message = L"Already logged in.";
                    server_socket_.SendPacketToClient(state.uniqueKey, response);
                    return;
                }

                Session* session = session_manager_.FindSessionByClientID(state.uniqueKey);
                if (session) session->CreatePlayer(unique_id);

                std::vector<CharacterInfo> characters;
                mysql_manager->ExecuteQuery(L"SELECT * FROM character_info WHERE account_unique_id = " + std::to_wstring(unique_id), [&](const sql::ResultSet* result)
                {
                    CharacterInfo character;
                    character.unique_id = result->getInt("unique_id");
                    character.account_unique_id = result->getInt("account_unique_id");
                    character.name = StringHelper::ToWideString(result->getString("name"));
                    character.lv = result->getInt("lv");
                    character.job = result->getInt("job");
                    character.last_position_x = static_cast<float>(result->getDouble("last_position_x"));
                    character.last_position_y = static_cast<float>(result->getDouble("last_position_y"));
                    characters.push_back(character);
                });

                LoginResponse response;
                response.is_success = true;
                response.message = L"Login successful.";
                response.characters = characters;
                server_socket_.SendPacketToClient(state.uniqueKey, response);

                session->SetState(Session::State::kLoggedIn);
            });

            if (!is_found)
            {
                LoginResponse response;
                response.is_success = false;
                response.message = L"Invalid ID or password.";
                server_socket_.SendPacketToClient(state.uniqueKey, response);
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
