#include <iostream>
#include <thread>
extern "C" int raw_lock(int *addr, int change, int expect) asm("raw_lock");

struct SpinLock
{
	SpinLock() : addr_(new int(0))
	{
	}
	void lock()
	{
		while (true)
		{
			int r = raw_lock(addr_, 1, 0);
			if (!r)
			{ // r is the same value with `expect` if *addr_ equals to 0(expect) since eax is not changed
				break;
			}
			// if *addr_ is not equivalent to 0(expect) then eax is changed to the value of *addr_
		}
	}
	void unlock()
	{
		int i = 0;
		while (true)
		{
			bool r = raw_lock(addr_, 0, 1);
			if (r)
			{
				break;
			}
			break;
		}
	}
	~SpinLock()
	{
		delete addr_;
	}
	SpinLock(SpinLock const &) = delete;

private:
	// static int raw_lock(int *addr, int change, int expect)
	// {
	// 	asm(
	// 		"movl %edx,%eax\r\n"
	// 		"movq %rdi,%rcx\r\n"
	// 		"movl %esi,%ebx\r\n"
	// 		"lock cmpxchg %ebx,(%rcx)");
	// }

private:
	int *addr_ = nullptr;
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
int main()
{
	auto lock = SpinLock{};
	for (auto index = 0; index < 100; index++)
	{
		int i = 0;
		auto t1 = std::thread([&lock, &i]()
							  {
		for(auto c = 0;c < 10;c++){
			{
				SpinLockGuard lock_guard{lock};
				i++;
			}
		} });
		auto t2 = std::thread([&lock, &i]()
							  {
		for(auto c = 0;c < 10;c++){
			{
				SpinLockGuard lock_guard{lock};
				i++;
			}
		} });
		auto t3 = std::thread([&lock, &i]()
							  {
		for(auto c = 0;c < 10;c++){
			{
				SpinLockGuard lock_guard{lock};
				i++;
			}
		} });
		t1.join();
		t2.join();
		t3.join();
		if (i != 30)
		{
			std::cout << i << std::endl;
		}
	}
}