#include "pch.h"
#include "WanderStrategy.h"

#include "Logger.h"

BT::WanderStrategy::WanderStrategy()
{
}

BT::Node::Status BT::WanderStrategy::Process()
{
    Logger::Print(L"WanderStrategy::Process");
    return Node::Status::kRunning;
}
