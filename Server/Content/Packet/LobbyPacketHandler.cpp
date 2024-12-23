#include "pch.h"
#include "LobbyPacketHandler.h"

#include "../ClientPacketHandler.h"
#include "../Lobby.h"
#include "../MatchingQueue.h"
#include "../../Database/MySQLConnectionPool.h"

void LobbyPacketHandler::HandleEnterToLobby(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToLobby> pkt)
{
}

void LobbyPacketHandler::HandleEnterToMatching(const shared_ptr<PacketSession>& session,
    shared_ptr<C_EnterToMatching> pkt)
{
    auto gameSession = static_pointer_cast<GameSession>(session);

    //TODO: Matching Queue Pool 제작 및 취소 기능 구현
    auto turn  = GMatchingQueue->Push(gameSession->userRef);

    
    S_EnterToMatching pkt;
    pkt._userId= gameSession->userRef->userIdentifyId;
    pkt._matchingQueueTurn=turn;
    pkt._matchingQueueId=0;
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_EnterToMatching>(pkt,S_PKT_ENTER_TO_MATCHING);
    session->Send(sendBuffer);
}

void LobbyPacketHandler::HandleMatchingCancel(const shared_ptr<PacketSession>& session,
    shared_ptr<C_MatchingCancel> pkt)
{
    //TODO: 매칭 취소 기능 구현하기
}

void LobbyPacketHandler::HandleJoinGame(const shared_ptr<PacketSession>& session, shared_ptr<C_JoinGame> pkt)
{
    auto gameSession = static_pointer_cast<GameSession>(session);

    auto currentRoom = GRoomMap->find(pkt->_mapId)->second;

    currentRoom->Enter(gameSession->userRef);

    S_JoinGame pkt;
    pkt._userId= gameSession->userRef->userIdentifyId;
    //TODO: 인게임 id 생성
    pkt._matchingQueueId=0;
    pkt._inGameId=0;

    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_JoinGame>(pkt,S_PKT_JOIN_GAME);
    session->Send(sendBuffer);
    
}

void LobbyPacketHandler::HandleSignIn(const shared_ptr<PacketSession>& session, shared_ptr<C_SignIn> pkt)
{
    S_SignIn signInPkt;
    string userName = pkt->_username;
    string password = pkt->_password;


    //ID중복확인
    {
        auto conn = GConnectionPool->acquire();
        sql::Statement* stmt = conn->get()->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT COUNT(*) AS CNT FROM USERS WHERE USER_ID="+userName + ";");
        while (res->next())
        {
            if (res->getInt("CNT")!=0)
            {
                //TODO: 회원가입 실패 패킷 쏘기
            }
        }
        delete stmt;
        delete res;

        GConnectionPool->release(conn);
    }

    //회원가입
    {
        auto conn = GConnectionPool->acquire();
        sql::Statement* stmt = conn->get()->createStatement();
        stmt->executeQuery("INSERT INTO USERS (USER_ID,USER_PASSWORD) VALUES('"+userName+"', '"+password+"');");
        delete stmt;
        GConnectionPool->release(conn);
    }
    signInPkt._isSuccess=1;
    signInPkt._username=userName;
    signInPkt._password=password;
    signInPkt._errorReason="empty";
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_SignIn>(signInPkt,S_PKT_SIGN_IN);
    session->Send(sendBuffer);
}

void LobbyPacketHandler::HandleLogin(const shared_ptr<PacketSession>& session, shared_ptr<C_Login> pkt)
{
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);
    S_Login loginPkt;

    string userName = pkt->_username;
    string password = pkt->_password;
    int userPk =0;
    {
        auto conn = GConnectionPool->acquire();
        sql::Statement* stmt = conn->get()->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM USERS WHERE USER_ID="+userName + ";");
        
        while (res->next())
        {
            if (res->getString("user_id") == userName && res->getString("user_password") == password)
            {
                userPk = res->getInt("user_pk");
            }
            else
            {
                //TODO: 로그인 실패 패킷
            }
        }
        delete stmt;
        delete res;
        GConnectionPool->release(conn);
    }

    //유저 정보 추가
    shared_ptr<User> user = make_shared<User>();
    user->name = userName;
    user->userIdentifyId = userPk;
    user->locationX=0.0f;
    user->locationY=0.0f;
    user->characterId = 1;
    user->ownerSession = gameSession;
    gameSession->userRef = user;

    //로비 들어가기
    globalLobby->Enter(user);
    //TODO: 로비 참가 브로드캐스팅

    loginPkt._isSuccess=1;
    loginPkt._username=userName;
    loginPkt._password=password;
    loginPkt._errorReason="empty";

    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_Login>(loginPkt,S_PKT_LOGIN);
    session->Send(sendBuffer);
    
}

void LobbyPacketHandler::HandleGetMyAwardInfo(const shared_ptr<PacketSession>& session,
    shared_ptr<C_GetMyAwardInfo> pkt)
{
    auto gameSession = static_pointer_cast<GameSession>(session);
    vector<Award> awards;
    {
        auto conn = GConnectionPool->acquire();
        sql::Statement* stmt = conn->get()->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM AWARDS WHERE USER_PK="+std::to_string(gameSession->userRef->userIdentifyId)+";");
        while (res->next())
        {
            Award award;
            award.awardPk=res->getInt("award_pk");
            award.awardName = res->getString("award_name");
            award.awardDescription=res->getString("award_description");
            awards.emplace_back(award);
        }
    }

    gameSession->userRef->awards = awards;

    S_GetMyAwardInfo awardPkt;
    awardPkt._userId = gameSession->userRef->userIdentifyId;
    awardPkt._awardCnt = awards.size();
    uint32_t* idArr = new uint32_t[awardPkt._awardCnt];
    string* nameArr = new string[awardPkt._awardCnt];
    string* descriptionArr = new string[awardPkt._awardCnt];
    int cnt=0;
    for (auto& award : awards)
    {
        idArr[cnt] = award.awardPk;
        nameArr[cnt] = award.awardName;
        descriptionArr[cnt] = award.awardDescription;
        cnt++;
    }
    awardPkt._awardIdArr = idArr;
    awardPkt._awardNameArr = nameArr;
    awardPkt._awardInfoArr = descriptionArr;

    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_GetMyAwardInfo>(awardPkt,S_PKT_GET_MY_AWARD_INFO);
    session->Send(sendBuffer);
    
}

void LobbyPacketHandler::HandleSelectCharacter(const shared_ptr<PacketSession>& session,
    shared_ptr<C_SelectCharacter> pkt)
{
    auto gameSession = static_pointer_cast<GameSession>(session);

    {
        auto conn = GConnectionPool->acquire();
        sql::Statement* stmt = conn->get()->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT COUNT(*) AS CNT FROM CHARACTERS WHERE USER_FK="+std::to_string(gameSession->userRef->userIdentifyId)+" AND CHARACTER_PK="+std::to_string(pkt->_characterId)+";");
        while (res->next())
        {
            if (res->getInt("CNT")==0)
            {
                //TODO: 아이템 선택 실패 메세지 보내기
            }
        }

        gameSession->userRef->characterId = pkt->_characterId;

        S_SelectCharacter selectCharPkt;
        selectCharPkt._userId = gameSession->userRef->userIdentifyId;
        selectCharPkt._isSuccess=1;
        selectCharPkt._characterId = pkt->_characterId;
        selectCharPkt._errorReason="empty";
        
        auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_SelectCharacter>(selectCharPkt,S_PKT_SELECT_CHARACTER);
        session->Send(sendBuffer);
    }
}

void LobbyPacketHandler::HandleSelectWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectWeapon> pkt)
{
    auto gameSession = static_pointer_cast<GameSession>(session);

    {
        auto conn = GConnectionPool->acquire();
        sql::Statement* stmt = conn->get()->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT COUNT(*) AS CNT FROM WEAPONS WHERE USER_FK="+std::to_string(gameSession->userRef->userIdentifyId)+" AND WEAPON_PK="+std::to_string(pkt->_weaponId)+";");
        while (res->next())
        {
            if (res->getInt("CNT")==0)
            {
                //TODO: 아이템 선택 실패 메세지 보내기
            }
        }

        gameSession->userRef->weaponId = pkt->_weaponId;

        S_SelectWeapon selectWeaPkt;
        selectWeaPkt._userId = gameSession->userRef->userIdentifyId;
        selectWeaPkt._isSuccess=1;
        selectWeaPkt._weaponId = pkt->_weaponId;
        selectWeaPkt._errorReason="empty";
        
        auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_SelectWeapon>(selectWeaPkt,S_PKT_SELECT_CHARACTER);
        session->Send(sendBuffer);
    }
}

void LobbyPacketHandler::HandleBuyWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_BuyWeapon> pkt)
{
}

void LobbyPacketHandler::HandleCacheFulling(const shared_ptr<PacketSession>& session, shared_ptr<C_CacheFulling> pkt)
{
}
