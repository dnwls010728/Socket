#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"
void JobQueue::Push(shared_ptr<Job> job, bool pushOnly)
{
	const int prevCount = _jobCount.fetch_add(1);
	_jobs.Push(job);

	if (prevCount == 0)
	{
		if (LCurrentJobQueue == nullptr && pushOnly == false)
		{
			Execute();
		}
		else
		{
			GGlobalQueue->Push(shared_from_this());
		}
	}
}

void JobQueue::Execute()
{
	LCurrentJobQueue = this;

	while (true)
	{
		vector<shared_ptr<Job>> jobs;
		_jobs.PopAll(OUT jobs);

		const size_t jobCount = jobs.size();

		for (int i = 0; i < jobCount; i++)
			jobs[i]->Execute();

		if (_jobCount.fetch_sub(jobCount) == jobCount)
		{
			LCurrentJobQueue = nullptr;
			return;
		}

		const uint64_t now = ::GetTickCount64();
		if (now >= LEndTickCount)
		{
			LCurrentJobQueue = nullptr;

			GGlobalQueue->Push(shared_from_this());
			break;
		}

	}
}
