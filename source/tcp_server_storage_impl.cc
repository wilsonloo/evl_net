#include "tcp_server_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPServerStorageImpl::TCPServerStorageImpl(boost::asio::io_service& io_service, 
			const char* ip,
			short port, 
			OnNewClientConnectedHandlerType on_new_client_connected_handler,
			OnDataReceivedHandlerType on_data_received_handler, 
			OnDataWrittenHandlerType on_data_written_handler,
			OnErrorHandlerType on_error_from_client_handler)
			: io_service_(io_service)
			, endpoint_(NULL)
			, acceptor_(NULL)
			, on_new_client_connected_handler_(on_new_client_connected_handler)
			, on_data_received_handler_(on_data_received_handler)
			, on_data_written_handler_(on_data_written_handler)
			, on_error_from_client_handler_(on_error_from_client_handler)
		{
			endpoint_ = new boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
			acceptor_ = new boost::asio::ip::tcp::acceptor(io_service_, *endpoint_);
		}

		TCPServerStorageImpl::~TCPServerStorageImpl()
		{
			if (acceptor_ != NULL)
			{
				if (acceptor_->is_open())
					acceptor_->close();

				delete acceptor_;
				acceptor_ = NULL;
			}

			if (endpoint_ != NULL)
			{
				delete endpoint_;
				endpoint_ = NULL;
			}
		}
	}
}
