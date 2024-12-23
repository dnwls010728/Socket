#pragma once

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	IocpObject() =default;
	virtual ~IocpObject() =default;
	IocpObject(const IocpObject&) =default;
	IocpObject(IocpObject&&) =default;
	IocpObject& operator=(const IocpObject&) =default;
	IocpObject& operator=(IocpObject&&) =default;
	
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numOfBytes = 0) abstract;
};


class IocpCore
{
public:
	IocpCore();
	~IocpCore();
	IocpCore(const IocpCore&) =default;
	IocpCore(IocpCore&&) =default;
	IocpCore& operator=(const IocpCore&) =default;
	IocpCore& operator=(IocpCore&&) =default;

	HANDLE GetHandle()const  { return _iocpHandle; }

	bool Register(const shared_ptr<IocpObject>& iocpObject) const;
	void Dispatch(uint32_t timeoutMs = INFINITE)const;

private:
	HANDLE _iocpHandle;
};

