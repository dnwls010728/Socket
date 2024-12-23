#pragma once
#include "..\Thread\Lock.h"

template<typename T>
class LockQueue
{
public:
	LockQueue() = default;
	~LockQueue() = default;

	LockQueue(const LockQueue&) = delete;
	LockQueue& operator=(const LockQueue&) = delete;
	LockQueue(LockQueue&&) = delete;
	LockQueue& operator=(LockQueue&&) = delete;
	
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	size_t Size()
	{
		return _items.size();
	}

	T Pop()
	{
		WRITE_LOCK;
		if (_items.empty())
			return T();

		T ret = _items.front();
		_items.pop();
		return ret;
	}

	void PopAll(OUT vector<T>& items)
	{
		WRITE_LOCK;
		while (1)
		{
			if (_items.empty())
				return;
			else
			{
				T item = _items.front();
				_items.pop();
				items.push_back(item);
			}

		}

	}

	void Clear()
	{
		WRITE_LOCK;
		_items = queue<T>();
	}

private:
	USE_LOCK;
	queue<T> _items;
};