#include "AsioThreadPool.h"



boost::asio::io_context& AsioThreadPool::GetIOService()
{
	return _service;
}

void AsioThreadPool::Stop()
{
	_work.reset();
	for (auto& t:_threads)
	{
		t.join();
	}
}

AsioThreadPool::AsioThreadPool(int threadNum):_work(new boost::asio::io_context::work(_service))
{
	for (size_t i = 0; i < threadNum; i++)
	{
		_threads.emplace_back([this]() {
			_service.run();//多个线程去运行这一个ioc
			});
	}
}
