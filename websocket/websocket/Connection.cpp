#include"Connection.h"
#include"ConnectionMgr.h"
//ConnectionMgr里包含了Connection的头文件，一定要在Connection里声明ConnectionMgr这个类

Connection::Connection(net::io_context& ioc_context):_ioc(ioc_context),
_ws_ptr(std::make_unique<stream<tcp_stream>>(make_strand(_ioc)))
{
	boost::uuids::random_generator generator;
	boost::uuids::uuid uuid = generator();
	_uuid = boost::uuids::to_string(uuid);
}

std::string Connection::GetUid()
{
	return _uuid;
}

net::ip::tcp::socket& Connection::GetSocket()
{
	auto& socket=beast::get_lowest_layer(*_ws_ptr).socket();
	return socket;
}

void Connection::AsyncAccept()
{
	auto self = shared_from_this();
	_ws_ptr->async_accept([self](boost::system::error_code err) {
		try
		{
			if (!err)
			{
				ConnectionMgr::GetInstance().AddConnection(self);
				self->Start();
			}
			else
			{
				std::cout << "WebSocket async accept failed, err is " << err.what() << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "WebSocket async accept exception is " << e.what() << std::endl;
		}
		});
}

void Connection::Start()
{
	auto self = shared_from_this();
	_ws_ptr->async_read(_recv_buffer, [self]
	(error_code e, std::size_t buffe_bytes) 
	{
		try
		{
			if (e)
			{
				std::cout << "WebSocket Async_read error is" << e.what() << std::endl;
				ConnectionMgr::GetInstance().RmvConnection(self->GetUid());
				return;
			}
			//走到这里代表读成功
			self->_ws_ptr->text(self->_ws_ptr->got_text());
			std::string recv_data = boost::beast::buffers_to_string(self->_recv_buffer.data());
			self->_recv_buffer.consume(self->_recv_buffer.size());
			std::cout << "recv_data is " << recv_data << std::endl;

			self->AsyncSend(std::move(recv_data));
			self->Start();
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception is " << e.what() << std::endl;
			ConnectionMgr::GetInstance().RmvConnection(self->GetUid());
		}
	});
}

void Connection::AsyncSend(std::string msg)
{
	{
		std::unique_lock<std::mutex> lock(_mutex);
		auto size = _send_que.size();
		_send_que.push(msg);
		if (size > 0)return;
	}

	auto self = shared_from_this();
	self->_ws_ptr->async_write(boost::asio::buffer(msg.c_str(),msg.length()), [self]
	(error_code e,std::size_t nsize) {
			try
			{
				if (e)
				{
					std::cout << "async_write error is " << e.what() << std::endl;
					ConnectionMgr::GetInstance().RmvConnection(self->GetUid());
					return;
				}
				
				std::string _msg;
				{
					std::unique_lock<std::mutex> lock(self->_mutex);
					self->_send_que.pop();
					if (self->_send_que.empty())return;
					_msg = self->_send_que.front();
				}
				
				self->AsyncSend(std::move(_msg));
			}
			catch (const std::exception& e)
			{
				std::cout << "Exception is " << e.what() << std::endl;
				ConnectionMgr::GetInstance().RmvConnection(self->GetUid());
			}
		});
}
