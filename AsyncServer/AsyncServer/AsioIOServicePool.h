#pragma once
#include<boost/asio.hpp>
#include<vector>
#include"Singleton.h"
class AsioIOServicePool:public Singleton< AsioIOServicePool>
{
	friend class Singleton< AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	/*这个服务器池子会启动多个ioc，如果ioc没有被注册事件
	.run()就会直接返回，绑定一个work就会等待*/
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	boost::asio::io_context& GetIOService();
	void stop();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	vector<IOService> _ioService;
	vector<WorkPtr> _Works;
	vector<std::thread> _threads;
	std::size_t _nextIOService;
};

