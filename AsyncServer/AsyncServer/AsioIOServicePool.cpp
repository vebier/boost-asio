#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioService(size), _Works(size), _nextIOService(0)
{
	for (size_t i = 0; i < size; i++)
	{
		_Works[i] = std::unique_ptr<Work>(new Work(_ioService[i]));
	}

	for (size_t i = 0; i < _ioService.size(); i++)
	{
		_threads.emplace_back([this,i]() {
			_ioService[i].run();
			});
	}
}
AsioIOServicePool::~AsioIOServicePool()
{
	std::cout << "call ~AsioIOServicePool" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService()
{
	auto& service = _ioService[_nextIOService++];
	if (_nextIOService == _ioService.size()) _nextIOService = 0;
	return service;
}

void AsioIOServicePool::stop()
{
	for (auto& work : _Works)
	{
		work.reset();
	}
	for (auto& t : _threads)
	{
		t.join();
	}

}


