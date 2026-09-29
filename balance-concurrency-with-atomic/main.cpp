#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<int> balance(1000);
void deposit(size_t amt);
void withdraw(size_t amt);

int main(int argc, char* argv[])
{
	std::vector<std::thread> depositThreadPool, withdrawThreadPool;

	if (argc < 2)
	{
		std::cout << "Missing thread count arg" << std::endl;
		return 1;
	}

	if (!balance.is_lock_free())
	{
		std::cout << "Balance atomic int is not lock free; Exiting" << std::endl;
		return 1;
	}

	size_t threads = std::atoi(argv[1]);
	std::cout << "Number of threads: " << threads << std::endl;
	std::cout << "Initial balance: " << balance.load() << std::endl;

	for (size_t i = 0; i < threads; i++)
	{
		withdrawThreadPool.emplace_back(withdraw, 100);
		depositThreadPool.emplace_back(deposit, 100);
	}

	for (auto& d : depositThreadPool) d.join();
	for (auto& w : withdrawThreadPool) w.join();

	std::cout << "Final balance: " << balance.load() << std::endl;
	return 0;
}

void deposit(size_t amt)
{
	for (size_t i = 0; i < 100; i++)
	{
		balance.fetch_add(amt);
	}
}

void withdraw(size_t amt)
{
	for (size_t i = 0; i < 100; i++)
	{
		balance.fetch_sub(amt);
	}
}