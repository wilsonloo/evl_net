#include "tcp_server_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPServerStorageImpl::TCPServerStorageImpl(boost::asio::io_service& io_service, short port, 
			OnNewClientConnectedHandlerType on_new_client_connected_handler,
			OnDataReceivedHandlerType on_data_received_handler, 
			OnDataWrittenHandlerType on_data_written_handler,
			OnErrorHandlerType on_error_from_client_handler)
			: io_service_(io_service)
			, acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
			, on_new_client_connected_handler_(on_new_client_connected_handler)
			, on_data_received_handler_(on_data_received_handler)
			, on_data_written_handler_(on_data_written_handler)
			, on_error_from_client_handler_(on_error_from_client_handler)
		{

		}
	}
}
