#pragma once

#include "..\pch.h"
#include "JobQueue.h"

class GlobalQueue
{
public:
	GlobalQueue() = default;
	~GlobalQueue() = default;
	GlobalQueue(const GlobalQueue&) = delete;
	GlobalQueue& operator=(const GlobalQueue&) = delete;
	GlobalQueue(GlobalQueue&&) = delete;
	GlobalQueue& operator=(GlobalQueue&&) = delete;

	void Push(const shared_ptr<JobQueue>& jobQueue);
	shared_ptr<JobQueue> Pop();
private:
	LockQueue<shared_ptr<JobQueue>> _jobQueues;
};

