#include "ServerManager.h"
#include <string>
#include <iostream>
#include "NetworkManager.h"
#include "CustomPacket.h"
#include "CustomSerializer.h"
#include "Session/Session.h"
#include "Helper/StringHelper.h"

ServerManager::ServerManager()
{
    server_socket_.SetSerializerFactory([]() { return std::make_unique<CustomSerializer>(); });

    // 콜벡 설정file:/D:/Users/Desktop/태양/게임엔진/Socket/Server/ServerManager.h
    server_socket_.SetClientAcceptedCallback(std::bind(&ServerManager::OnClientConnected, this, std::placeholders::_1));
    server_socket_.SetClientDisconnectedCallback(std::bind(&ServerManager::OnClientDisconnected, this, std::placeholders::_1));
    server_socket_.SetPacketReceivedCallback(std::bind(&ServerManager::OnPacketReceived, this, std::placeholders::_1, std::placeholders::_2));
}

bool ServerManager::Execute()
{
    if (!mysql_manager_.Connect("poroserver.iptime.org", "y_eternal", "@eternal12345"))
    {
        return false;
    }

    Net::WSAInit();

    Net::NetAddress server_address("0.0.0.0", 9000);
    if (server_socket_.Start(server_address, 0) == false)
    {
        return false;
    }

    // 명령어 입력 루프
    bool stop_flag = false;
    while (stop_flag == false)
    {
        std::string command;
        std::cin >> command;

        std::string first_token;
        size_t token_idx = command.find(' ', 0);
        if (token_idx != std::string::npos)
        {
            first_token = command.substr(0, token_idx);
        }
        else
        {
            first_token = command;
        }

        if (first_token == "/exit")
        {
            stop_flag = true;
        }
        else if (first_token == "/help")
        {
            std::cout << "Available commands:" << std::endl;
            std::cout << "/exit - Exits the server." << std::endl;
        }
        else
        {
            std::cout << "Error: Unrecognized command. Please check '/help' for a list of available commands." << std::endl;
        }
    }

    server_socket_.Stop();
    Net::WSAUninit();
    mysql_manager_.Disconnect();
    return true;
}

bool ServerManager::OnClientConnected(const Net::TCPConnectionState& state)
{
    std::cout << "Client connected: " << state.address.ToString().c_str() << std::endl;
    std::shared_ptr<Session> session = std::make_shared<Session>(state.uniqueKey);
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
    switch (packet->GetPacketID())
    {
    case MessagePacket::StaticPacketID:
        {
            MessagePacket* message_packet = static_cast<MessagePacket*>(packet.get());
            std::cout << "Message received from " << state.address.ToString().c_str() << ": " << message_packet->message.c_str() << std::endl;
        }
        break;

    case RegisterRequest::StaticPacketID:
        {
            RegisterRequest* request = static_cast<RegisterRequest*>(packet.get());

            bool is_found = false;
            mysql_manager_.ExecuteQuery(L"SELECT * FROM account_info WHERE id = '" + request->id + L"'", [&](const sql::ResultSet* result)
            {
                is_found = true;

                RegisterResponse response;
                response.is_success = false;
                response.message = L"ID already exists.";
                server_socket_.SendPacketToClient(state.uniqueKey, response);
            });

            if (!is_found)
            {
                int result = mysql_manager_.ExecuteUpdate(L"INSERT INTO account_info (id, password) VALUES ('" + request->id + L"', '" + request->password + L"')");
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

            bool is_found = false;
            mysql_manager_.ExecuteQuery(L"SELECT * FROM account_info WHERE id = '" + request->id + L"' AND password = '" + request->password + L"'", [&](const sql::ResultSet* result)
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

                std::shared_ptr<Session> session = session_manager_.FindSessionByClientID(state.uniqueKey);
                if (session) session->SetAccountUniqueID(unique_id);

                std::vector<CharacterInfo> characters;
                mysql_manager_.ExecuteQuery(L"SELECT * FROM character_info WHERE account_unique_id = " + std::to_wstring(unique_id), [&](const sql::ResultSet* result)
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

    case SelectCharacterRequest::StaticPacketID:
        {
            SelectCharacterRequest* request = static_cast<SelectCharacterRequest*>(packet.get());

            std::shared_ptr<Session> session = session_manager_.FindSessionByClientID(state.uniqueKey);
            if (session)
            {
                // 해당 캐릭터가 로그인한 계정의 캐릭터인지 확인
                bool is_found = false;
                mysql_manager_.ExecuteQuery(L"SELECT * FROM character_info WHERE unique_id = " + std::to_wstring(request->unique_id) + L" AND account_unique_id = " + std::to_wstring(session->GetAccountUniqueID()), [&](const sql::ResultSet* result)
                {
                    is_found = true;
                });

                if (is_found)
                {
                    session->SetCharacterUniqueID(request->unique_id);

                    SelectCharacterResponse response;
                    response.is_success = true;
                    response.message = L"Character selected successfully.";
                    server_socket_.SendPacketToClient(state.uniqueKey, response);
                    break;
                }
            }

            SelectCharacterResponse response;
            response.is_success = false;
            response.message = L"Character selection failed.";
            server_socket_.SendPacketToClient(state.uniqueKey, response);
        }
        break;

    default:
        break;
    }
}
