#include "tcp_session_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPSessionStorageImpl::TCPSessionStorageImpl(OnDataReceivedHandlerType on_data_received_handler, 
													OnDataWrittenHandlerType on_data_written_handler, 
													OnErrorHandlerType on_error_from_client_handler)
													: on_data_received_handler_(on_data_received_handler)
													, on_data_written_handler_(on_data_written_handler)
													, on_error_from_client_handler_(on_error_from_client_handler)
		{

		}
	}
}
