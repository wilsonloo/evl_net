#ifndef EVL_NET_TCP_CLIENT_STORAGE_IMPL_H_
#define EVL_NET_TCP_CLIENT_STORAGE_IMPL_H_

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include "evl_net/evl_net_def.h"

namespace evl
{
	namespace net
	{
		class TCPClientStorageImpl
		{
			friend class EVL_NET_DECL TCPClient;
		public:
			TCPClientStorageImpl(boost::asio::io_service& io_service,
				OnConnectedToServerHandlerType on_connected_to_server_handler,
				OnDataReceivedHandlerType on_data_received_handler, 
				OnDataWrittenHandlerType on_data_written_handler,
				OnErrorHandlerType on_error_from_server_handler);

			inline void set_resolver(boost::asio::ip::tcp::resolver* res)
			{
				resolver_.reset(res);
			}

			inline void set_query(boost::asio::ip::tcp::resolver::query* que)
			{
				query_.reset(que);
			}

			inline boost::asio::ip::tcp::resolver::iterator get_resolver_iterator()
			{
				return resolver_->resolve(*query_.get());
			}

		private:
			boost::asio::io_service& io_service_;
			boost::asio::ip::tcp::socket socket_;

			boost::scoped_ptr<boost::asio::ip::tcp::resolver> resolver_;
			boost::scoped_ptr<boost::asio::ip::tcp::resolver::query> query_;

			OnConnectedToServerHandlerType on_connected_to_server_handler_;
			OnDataReceivedHandlerType on_data_received_handler_;
			OnDataWrittenHandlerType on_data_written_handler_;
			OnErrorHandlerType on_error_from_server_handler_;
		};
	} // namespace net
} // namespace evl

#endif // EVL_NET_TCP_SERVER_STORAGE_IMPL_H_
