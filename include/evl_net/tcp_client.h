#ifndef EVL_NET_TCP_CLIENT_H_
#define EVL_NET_TCP_CLIENT_H_

#include <boost/asio.hpp>
#include "evl_net_def.h"

namespace evl
{
	namespace net
	{
		class EVL_NET_DECL TCPSession;
		class TCPClientStorageImpl;

		class EVL_NET_DECL TCPClient
		{
			typedef boost::function<void (void* dest, const void* source, size_t length)> UserDefinedDataCopyHandlerType;

		public:
			TCPClient(boost::asio::io_service& io_service,  
				OnConnectedToServerHandlerType on_connected_to_server_handler, 
				OnDataReceivedHandlerType on_data_received_from_server_handler, 
				OnDataWrittenHandlerType on_data_written_to_server_handler,
				OnErrorHandlerType on_error_from_client_handler);

			virtual ~TCPClient();

			void StartConnect(const char* host, short port);
			inline bool is_valid()
			{
				return session_ != NULL;
			}

			void Close();

			inline TCPSession* get_session(){ return session_; }

			inline void set_user_data(void* user_data){ user_data_ = user_data; }
			template<typename TargetT>
			TargetT* get_user_data()
			{
				return reinterpret_cast<TargetT*>(user_data_);
			}

			void SendMsg(const void* msg, size_t msg_length, bool delay_send = false);
			void SendMsg(const void* msg, size_t msg_length, UserDefinedDataCopyHandlerType copy_handler);

		private:
			void HandleConnect(const boost::system::error_code& err);

		private:
			TCPSession* session_;
			TCPClientStorageImpl* storage_impl_;
			void* user_data_;
		};
	} // namespace net
} // namespace evl

#endif // EVL_NET_TCP_CLIENT_H_