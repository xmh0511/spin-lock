
extern "C" char raw_lock(char *addr, char change, char expect) asm("raw_lock");

struct SpinLock
{
	SpinLock() : addr_(new char(0))
	{
	}
	void lock()
	{
		while (true)
		{
			char r = raw_lock(addr_, 1, 0);
			if (!r)
			{ // r is the same value with `expect` if *addr_ equals to 0(expect) since eax is not changed
				break;
			}
			// if *addr_ is not equivalent to 0(expect) then eax is changed to the value of *addr_
		}
	}
	void unlock()
	{
		while (true)
		{
			char r = raw_lock(addr_, 0, 1);
			if (r)
			{
				break;
			}
		}
	}
	~SpinLock()
	{
		delete addr_;
	}
	SpinLock(SpinLock const &) = delete;

private:
	char *addr_ = nullptr;
};
class SpinLockGuard
{
public:
	SpinLockGuard(SpinLock &lock) : lock_(lock)
	{
		lock.lock();
	}
	~SpinLockGuard()
	{
		lock_.unlock();
	}

private:
	SpinLock &lock_;
};