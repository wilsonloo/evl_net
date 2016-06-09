#ifndef EVL_NET_TCP_SESSION_H_
#define EVL_NET_TCP_SESSION_H_

#include <boost/asio.hpp>
#include <boost/asio/basic_socket.hpp>
#include <boost/function.hpp>
#include <third_party/utility/read_write_array.hpp>
#include "evl_net_def.h"

namespace evl
{
	namespace net
	{
		class TCPSessionStorageImpl;
		class EVL_NET_DECL TCPServer;
		class EVL_NET_DECL TCPClient;
		class EVL_NET_DECL TCPSession
		{
		public:
			typedef boost::function<void (void* dest, const void* source, size_t length)> UserDefinedDataCopyHandlerType;
			typedef evl::net::UUIDType UUIDType;
			typedef boost::asio::ip::tcp::socket::endpoint_type EndpointType;

		public:
			TCPSession(boost::asio::io_service& io_service,
				OnDataReceivedHandlerType on_data_received_handler,
				OnDataWrittenHandlerType on_data_written_handler, 
				OnErrorHandlerType on_error_from_client_handler);

			TCPSession(boost::asio::io_service& io_service,
				boost::asio::ip::tcp::socket* socket,
				OnDataReceivedHandlerType on_data_received_handler,
				OnDataWrittenHandlerType on_data_written_handler, 
				OnErrorHandlerType on_error_from_client_handler);
			
			virtual ~TCPSession();

			inline boost::asio::ip::tcp::socket& socket()
			{
				return *socket_;
			}

			inline void set_user_data(void* user_data){ user_data_ = user_data; }
			template<typename TargetT>
			TargetT* get_user_data()
			{
				return reinterpret_cast<TargetT*>(user_data_);
			}

			inline void set_uuid(UUIDType uuid){ uuid_ = uuid;}
			inline UUIDType get_uuid(){ return uuid_;}

			inline void set_tcp_client(TCPClient* client){ tcp_client_ = client; }
			inline TCPClient* get_tcp_client(){ return tcp_client_; }

			inline void set_tcp_server(TCPServer* server){ tcp_server_ = server; }
			inline TCPServer* get_tcp_server(){ return tcp_server_; }

			inline EndpointType get_remote_endpoint()const
			{
				return socket_->remote_endpoint();
			}

			void Start();
			void Close();
			void SendMsg(const void* msg, size_t msg_length, bool delay_send = false);
			void SendMsg(const void* msg, size_t msg_length, UserDefinedDataCopyHandlerType copy_handler);
			void _send_msg();
			void Flush();

		private:
			void HandleRead(const boost::system::error_code& err, size_t bytes_transfered);
			void HandleWrite(const char* data, size_t bytes_transferred, const boost::system::error_code& err, size_t bytes_transfered);
			
		private:
			boost::asio::io_service& io_service_;
			boost::asio::ip::tcp::socket* socket_;
			bool socket_self_created_;
			char data_[MAX_DATA_LENGTH_PER_PACKET];
			void* user_data_;
			evl::utility::ReadWriteArray<MAX_DATA_LENGTH_PER_PACKET * 6>* writing_data_;
			
			// 
			UUIDType uuid_;

		private:
			TCPSessionStorageImpl* storage_impl_;

			union
			{
				TCPClient* tcp_client_;
				TCPServer* tcp_server_;
			};
		}; // class EVL_NET_DECL TCPSession
	} // namespace net
} // namespace evl

#endif // EVL_NET_TCP_SESSION_H_
