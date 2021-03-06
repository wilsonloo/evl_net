#ifndef EVL_NET_TCP_SESSION_STORAGE_IMPL_H_
#define EVL_NET_TCP_SESSION_STORAGE_IMPL_H_

#include "evl_net/evl_net_def.h"

namespace evl
{
	namespace net
	{
		class EVL_NET_DECL TCPSession;

		class TCPSessionStorageImpl
		{
			friend class EVL_NET_DECL TCPSession;

		public:
			TCPSessionStorageImpl(OnDataReceivedHandlerType on_data_received_handler,
									OnDataWrittenHandlerType on_data_written_handler, 
									OnErrorHandlerType on_error_from_client_handler);

		private:
			OnDataReceivedHandlerType on_data_received_handler_;
			OnDataWrittenHandlerType on_data_written_handler_;
			OnErrorHandlerType on_error_from_client_handler_;
		};
	} // namespace net
} // namespace evl

#endif // EVL_NET_TCP_SESSION_STORAGE_IMPL_H_
