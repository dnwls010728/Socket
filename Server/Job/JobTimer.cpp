#include "pch.h"
#include "JobTimer.h"


void JobTimer::Reserve(uint64_t tickAfter, const weak_ptr<JobQueue>& owner, const shared_ptr<Job>& job)
{
	const uint64_t executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = new JobData(owner, job);

	WRITE_LOCK;

	_items.push(TimerItem{ executeTick,jobData });
}

void JobTimer::Distribute(uint64_t now)
{
	if (_distributing.exchange(true) == true)
		return;

	vector<TimerItem> items;

	{
		WRITE_LOCK;

		while (_items.empty() == false)
		{
			const TimerItem& timerItem = _items.top();
			if (now < timerItem.executeTick)
				break;

			items.push_back(timerItem);
			_items.pop();
		}
	}

	for (TimerItem& item : items)
	{
		if (shared_ptr<JobQueue> owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job);

		delete item.jobData;
	}

	_distributing.store(false);
}

void JobTimer::Clear()
{
	WRITE_LOCK;

	while (_items.empty() == false)
	{
		const TimerItem& timerItem = _items.top();
		delete timerItem.jobData;
		_items.pop();
	}
}
