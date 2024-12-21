#pragma once


#include "Job.h"
#include "JobQueue.h"
struct JobData
{
	JobData(const weak_ptr<JobQueue>& owner, const shared_ptr<Job>& job) : owner(owner), job(job)
	{

	}

	weak_ptr<JobQueue> owner;
	shared_ptr<Job> job;

		
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64_t executeTick = 0;
	JobData* jobData = nullptr;
};

class JobTimer
{
public:
	void Reserve(uint64_t tickAfter, const weak_ptr<JobQueue>& owner, const shared_ptr<Job>& job);
	void Distribute(uint64_t now);
	void Clear();

private:
	USE_LOCK;
	priority_queue<TimerItem> _items;
	atomic<bool> _distributing = false;



};

