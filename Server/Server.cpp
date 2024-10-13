#include "pch.h"

#include "..\Content\GameSession.h";
#include "..\Content\ClientPacketHandler.h"

enum
{
    WORKER_TICK = 64
};
void DoWorkerJob(shared_ptr<ServerService>& service)
{
    while (true)
    {
        LEndTickCount = ::GetTickCount64() + WORKER_TICK;
        service->GetIocpCore()->Dispatch(10);
        ThreadManager::DistributeReservedJobs();
        ThreadManager::DoGlobalQueueWork();
    }
}
int main()
{
    ClientPacketHandler::Init();


    shared_ptr<ServerService> service = make_shared<ServerSerivce>(
        NetworkConnector(L"127.0.0.1", 7777),
        make_shared<IocpCore>(),
        static_cast<SessionFactory>(make_shared<GameSession>),
        10000
    );
    service->Start();

    //TODO: CPU의 코어 수 * 2 + 1개로 맞추기
    for (int i = 0; i < 17; i++)
    {
        GThreadManager->Launch([&service]()
            {
                DoWorkerJob(service);
            });
    }
    DoWorkerJob(service);
    GThreadManager->Join();
    return 0;
}

