#include "evl_net/tcp_server.h"
#include "evl_net/detail/net_mgr.h"
#include <boost/bind.hpp>
#include <evl_logger/evl_logger.h>
#include "evl_net/evl_net_def.h"
#include "evl_net/tcp_session.h"
#include "tcp_server_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPServer::TCPServer(boost::asio::io_service& io_service, boost::uint16_t port,
			OnNewClientConnectedHandlerType on_new_client_connected_handler, 
			OnDataReceivedHandlerType on_data_received_handler, 
			OnDataWrittenHandlerType on_data_written_handler, 
			OnErrorHandlerType on_error_from_client_handler)
			: port_(port)
			, storage_impl_(NULL)
		{
			storage_impl_ = new TCPServerStorageImpl(io_service, port, 
				on_new_client_connected_handler, 
				on_data_received_handler, 
				on_data_written_handler,
				on_error_from_client_handler);	

			storage_impl_->uuid_generator_.reset(new evl::utility::SequenceUUIDGenerator<UUIDType>(UUID_MIN_ID, UUID_MAX_ID, UUID_INVALID_ID));
		}

		TCPServer::~TCPServer()
		{
			if(storage_impl_ != NULL)
			{
				delete storage_impl_;
				storage_impl_ = NULL;
			}
		}

		void TCPServer::StartAccept()
		{
			EVL_LOG_DEBUG (sNetMgr.get_evl_logger(), "listening for new client to connect @ port:" << static_cast<unsigned int>(port_));

			TCPSession* new_session = new TCPSession(storage_impl_->io_service_,
													storage_impl_->on_data_received_handler_, 
													storage_impl_->on_data_written_handler_, 
													storage_impl_->on_error_from_client_handler_);
			new_session->set_tcp_server(this);
			UUIDType uuid = storage_impl_->uuid_generator_->generate();
			if(uuid == storage_impl_->uuid_generator_->invalid_id())
			{
				EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "failed to get uuid for new TCPSession.");
				return;
			}
			new_session->set_uuid(uuid);

			storage_impl_->acceptor_.async_accept(new_session->socket(), 
				boost::bind(&TCPServer::HandleAccept, this, new_session, 
				boost::asio::placeholders::error));
		}

		void TCPServer::HandleAccept(TCPSession* new_session, 
									const boost::system::error_code& err)
		{
			if(!err)
			{
				EVL_LOG_INFO(sNetMgr.get_evl_logger(), "new client connected " << new_session->get_remote_endpoint().address().to_v4().to_string()
					<< ":" << new_session->get_remote_endpoint().port());

				storage_impl_->on_new_client_connected_handler_(new_session);
				new_session->Start();
			}
			else
			{
				EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "failed to recieve client session:" << &new_session << " with error:" << err.message());

				if(new_session != NULL)
				{
					delete new_session;
				}
			}

			StartAccept();
		}
	} // namespace net
} // namespace evl
