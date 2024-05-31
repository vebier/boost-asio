#include"Server.h"
#include"ConnectionMgr.h"

Server::Server(net::io_context& ioc, unsigned short port) :_ioc(ioc),
_acceptor(ioc, net::ip::tcp::endpoint(net::ip::tcp::v4(), port))
{
	std::cout << "Server start on port : " << port << std::endl;
}

void Server::StartAccept()
{
	auto con_ptr = std::make_shared<Connection>(_ioc);
	_acceptor.async_accept(con_ptr->GetSocket(), [this, con_ptr](error_code err) {
		try {
			if (!err) {
				con_ptr->AsyncAccept();
			}
			else {
				std::cout << "acceptor async_accept failed, err is " << err.what() << std::endl;
			}

			StartAccept();
		}
		catch (std::exception& exp) {
			std::cout << "async_accept error is " << exp.what() << std::endl;
		}
		});
}

