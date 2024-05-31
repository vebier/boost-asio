#pragma once
#include"ConnectionMgr.h"
class Server
{
public:
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	Server(net::io_context& ioc,unsigned short port);
	void StartAccept();
private:
	net::io_context& _ioc;
	net::ip::tcp::acceptor _acceptor;
};

