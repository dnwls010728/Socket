#pragma once

#include "Job.h"
#include "..\Util\LockQueue.h"
#include "..\Job\JobTimer.h"
class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	void DoAsync(CallbackType&& callback)
	{
		Push(make_shared<Job>(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(make_shared<Job>(owner,memFunc,std::forward<Args>(args)...));
	}

	void DoTimer(uint64_t tickAfter, CallbackType&& callback)
	{
		shared_ptr<Job> job = make_shared<Job>(std::move(callback));
		GJobTimer->Reserve(tickAfter,shared_from_this(),job);
	}
	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64_t tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T>  owner = static_pointer_cast<T>(shared_from_this());
		shared_ptr<Job> job = make_shared<Job>(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter,shared_from_this(),job);
	}

	void ClearJobs() { _jobs.Clear(); }

public:
	void Push(shared_ptr<Job> job,bool pushOnly = true);
	void Execute();

protected:
	LockQueue<shared_ptr<Job>> _jobs;
	atomic<int> _jobCount;
};

