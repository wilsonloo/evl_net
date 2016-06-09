#ifndef EVL_NET_TCP_SERVER_H_
#define EVL_NET_TCP_SERVER_H_

#include <boost/cstdint.hpp>
#include <boost/asio.hpp>
#include "evl_net_def.h"

namespace evl
{
	namespace net
	{
		class PacketCoding;

		class EVL_NET_DECL TCPSession;
		class TCPServerStorageImpl;

		class EVL_NET_DECL TCPServer
		{
		public:
			TCPServer(boost::asio::io_service& io_service, 
				boost::uint16_t port, 
				OnNewClientConnectedHandlerType on_new_client_connected_handler,
				OnDataReceivedHandlerType on_data_received_handler,
				OnDataWrittenHandlerType on_data_written_handler, 
				OnErrorHandlerType on_error_from_client_handler);

			virtual ~TCPServer();

			// 开始接收网络连接
			void StartAccept();

			template<typename PC_TYPE>
			void SetPacketCoding()
			{
				packet_coding_ = new PC_TYPE();
			}

		private:
			void HandleAccept(TCPSession* new_session, 
				const boost::system::error_code& err);

			// 端口
			boost::uint16_t port_;

		private:
			TCPServer(const TCPServer&);

		private:
			TCPServerStorageImpl* storage_impl_;
			PacketCoding* packet_coding_;
		}; // class TCPServer

	} // namespace net
} // namespace evl

#endif // EVL_NET_TCP_SERVER_H_