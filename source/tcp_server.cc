#include "evl_net/tcp_server.h"
#include "evl_net/detail/net_mgr.h"
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <evl_logger/evl_logger.h>
#include "evl_net/evl_net_def.h"
#include "evl_net/tcp_session.h"
#include "tcp_server_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPServer::TCPServer(boost::asio::io_service& io_service, 
			const char* ip,
			boost::uint16_t port,
			OnNewClientConnectedHandlerType on_new_client_connected_handler, 
			OnDataReceivedHandlerType on_data_received_handler, 
			OnDataWrittenHandlerType on_data_written_handler, 
			OnErrorHandlerType on_error_from_client_handler)
			: ip_(ip)
			, port_(port)
			, storage_impl_(NULL)
		{
			storage_impl_ = new TCPServerStorageImpl(io_service, 
				ip_.c_str(),
				port_, 
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
			// EVL_LOG_DEBUG (sNetMgr.get_evl_logger(), "listening for new client to connect @ port:" << static_cast<unsigned int>(port_));

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

			BOOST_ASSERT(storage_impl_->acceptor_ != NULL);
			storage_impl_->acceptor_->async_accept(new_session->socket(), 
				boost::bind(&TCPServer::HandleAccept, this, new_session, 
				boost::asio::placeholders::error));
		}

		void TCPServer::HandleAccept(TCPSession* new_session, 
									const boost::system::error_code& err)
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "handling new accept...");

			if(err)
			{
				EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "failed to recieve client session:" << &new_session << " with error:" << err.message());


				new_session->Shutdown();
				delete new_session;
				new_session = NULL;
				return;
			}
			
			// as HandleAccept() is a async event, session may be invalid now
			try
			{
				EVL_LOG_INFO(sNetMgr.get_evl_logger(), "new client connected " << new_session->get_remote_endpoint().address().to_v4().to_string()
					<< ":" << new_session->get_remote_endpoint().port());

				storage_impl_->on_new_client_connected_handler_(new_session);
				new_session->Start();
			}
			catch (const boost::system::system_error& ex)
			{
				new_session->Shutdown();
				delete new_session;
				new_session = NULL;

				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "handling new accept...Failed:" << ex.what());
			}
			catch (const boost::exception& ex)
			{
				new_session->Shutdown();
				delete new_session;
				new_session = NULL;
				
				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "handling new accept...Failed:" << boost::diagnostic_information(ex)
					<< boost::diagnostic_information_what(ex));
			}

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "handling new accept...Done");
			StartAccept();
		}
	} // namespace net
} // namespace evl
