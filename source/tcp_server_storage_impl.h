#ifndef EVL_NET_TCP_SERVER_STORAGE_IMPL_H_
#define EVL_NET_TCP_SERVER_STORAGE_IMPL_H_

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <third_party/utility/uuid_generator.hpp>
#include "evl_net/evl_net_def.h"

namespace evl
{
	namespace net
	{
		class TCPServer;

		class TCPServerStorageImpl
		{
			friend class TCPServer;
		public:
			TCPServerStorageImpl(boost::asio::io_service& io_service, 
				const char* ip,
				short port, 
				OnNewClientConnectedHandlerType on_new_client_connected_handler,
				OnDataReceivedHandlerType on_data_received_handler, 
				OnDataWrittenHandlerType on_data_written_handler,
				OnErrorHandlerType on_error_from_client_handler);

			virtual ~TCPServerStorageImpl();

		private:
			boost::asio::io_service& io_service_;
			boost::asio::ip::tcp::endpoint* endpoint_;
			boost::asio::ip::tcp::acceptor* acceptor_;

			OnNewClientConnectedHandlerType on_new_client_connected_handler_;
			OnDataReceivedHandlerType on_data_received_handler_;
			OnDataWrittenHandlerType on_data_written_handler_;
			OnErrorHandlerType on_error_from_client_handler_;

			// Î¨Ò»IDÉú³ÉÆ÷
			boost::scoped_ptr<evl::utility::UUIDGenerator<std::size_t> > uuid_generator_;
		};
	} // namespace net
} // namespace evl

#endif // EVL_NET_TCP_SERVER_STORAGE_IMPL_H_
