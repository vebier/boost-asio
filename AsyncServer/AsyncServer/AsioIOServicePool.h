#pragma once
#include<boost/asio.hpp>
#include<vector>
#include"Singleton.h"
class AsioIOServicePool:public Singleton< AsioIOServicePool>
{
	friend class Singleton< AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	/*������������ӻ��������ioc�����iocû�б�ע���¼�
	.run()�ͻ�ֱ�ӷ��أ���һ��work�ͻ�ȴ�*/
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

