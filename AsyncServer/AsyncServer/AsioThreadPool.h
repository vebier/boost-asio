#pragma once
#include"Singleton.h"
#include<boost/asio.hpp>
class AsioThreadPool:public Singleton<AsioThreadPool>
{
	friend class Singleton<AsioThreadPool>;
public:
	~AsioThreadPool(){}
	AsioThreadPool(const AsioThreadPool&) = delete;
	AsioThreadPool& operator=(const AsioThreadPool&) = delete;
	boost::asio::io_context& GetIOService();
	void Stop(); 
private:
	AsioThreadPool(int threadNum = std::thread::hardware_concurrency());
	boost::asio::io_context _service;
	std::vector<std::thread> _threads;
	std::unique_ptr<boost::asio::io_context::work> _work;
	
};

