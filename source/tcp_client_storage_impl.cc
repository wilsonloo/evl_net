#include "tcp_client_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPClientStorageImpl::TCPClientStorageImpl(boost::asio::io_service& io_service, 
													OnConnectedToServerHandlerType on_connected_to_server_handler, 
													OnDataReceivedHandlerType on_data_received_handler, 
													OnDataWrittenHandlerType on_data_written_handler, 
													OnErrorHandlerType on_error_from_server_handler)
													: io_service_(io_service)
													, socket_(io_service)
													, on_connected_to_server_handler_(on_connected_to_server_handler)
													, on_data_received_handler_(on_data_received_handler)
													, on_data_written_handler_(on_data_written_handler)
													, on_error_from_server_handler_(on_error_from_server_handler)
		{
			resolver_.reset(static_cast<boost::asio::ip::tcp::resolver*>(NULL));
			query_.reset(static_cast<boost::asio::ip::tcp::resolver::query*>(NULL));
		}
	}
}