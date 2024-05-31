#pragma once
#include <iostream>
#include<boost/beast.hpp>
#include<boost/asio.hpp>
#include<boost/uuid/uuid.hpp>
#include<boost/uuid/uuid_io.hpp>
#include<boost/uuid/uuid_generators.hpp>
#include<memory.h>
#include<queue>
#include<mutex>

namespace net = boost::asio;
namespace beast = boost::beast;
using namespace boost::beast;
using namespace boost::beast::websocket;


class Connection :public std::enable_shared_from_this<Connection>
{
public:
	Connection(net::io_context& ioc_context);
	std::string GetUid();
	net::ip::tcp::socket& GetSocket();
	void AsyncAccept();//读客户端发送的数据
	void Start();
	void AsyncSend(std::string msg);//发送数据给客户端
private:
	net::io_context& _ioc;
	std::string _uuid;
	std::unique_ptr<stream<tcp_stream>> _ws_ptr;//socket
	std::queue<std::string> _send_que;
	flat_buffer _recv_buffer;
	std::mutex _mutex;
};
