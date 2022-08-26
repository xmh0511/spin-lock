#include <iostream>
#include <thread>
#include "spin_lock.hpp"
int main()
{
	auto lock = SpinLock{};
	for (auto index = 0; index < 888888; index++)
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
		auto t3 = std::thread([&lock, &i](){
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