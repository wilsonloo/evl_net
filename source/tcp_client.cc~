#include "evl_net/tcp_client.h"
#include "evl_net/detail/net_mgr.h"
#include <boost/bind.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "evl_net/evl_net_def.h"
#include "evl_net/tcp_session.h"
#include "tcp_client_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPClient::TCPClient(boost::asio::io_service& io_service, 
							OnConnectedToServerHandlerType on_connected_to_server_handler, 
							OnDataReceivedHandlerType on_data_received_from_server_handler, 
							OnDataWrittenHandlerType on_data_written_to_server_handler, 
							OnErrorHandlerType on_error_from_client_handler)
							: storage_impl_(NULL)
							, session_(NULL)
							, user_data_(NULL)
		{
			storage_impl_ = new TCPClientStorageImpl(io_service, 
													on_connected_to_server_handler, 
													on_data_received_from_server_handler, 
													on_data_written_to_server_handler, 
													on_error_from_client_handler);
		}

		TCPClient::~TCPClient()
		{
			EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "TCPClient disposed");

			if(storage_impl_ != NULL)
			{
				delete storage_impl_;
				storage_impl_ = NULL;
			}
		}

		void TCPClient::StartConnect(const char* host, short port)
		{
			using namespace boost::asio::ip;

			std::stringstream ss;
			std::string port_s;
			std::string host_s;

#define READ_STRINGSTREAM(value, output) \
			ss.clear(); ss << value; ss >> output;

			READ_STRINGSTREAM(host, host_s);
			READ_STRINGSTREAM(static_cast<unsigned int>(port), port_s);

#undef READ_STRINGSTREAM

			try{
				storage_impl_->set_resolver(new tcp::resolver(storage_impl_->io_service_));
				storage_impl_->set_query(new tcp::resolver::query(tcp::v4(), host_s, port_s));
				tcp::resolver::iterator iter = storage_impl_->get_resolver_iterator();

				/*
				storage_impl_->socket_.async_connect(
					tcp::endpoint(boost::asio::ip::address_v4::from_string(host_s), port), 
					boost::bind(&TCPClient::HandleConnect, this, boost::asio::placeholders::error));
					*/

				boost::asio::async_connect(storage_impl_->socket_, iter,
					boost::bind(&TCPClient::HandleConnect, this,
					boost::asio::placeholders::error));
			}
			catch(const boost::exception& ex)
			{
				EVL_LOG_DEBUG (sNetMgr.get_evl_logger(), "failed to connect server.error:" << boost::diagnostic_information(ex)
					<< boost::diagnostic_information_what(ex));
			}
		}

		void TCPClient::Close()
		{
			if (session_ != NULL)
			{
				session_->Close();
				delete session_;
				session_ = NULL;
			}
		}

		void TCPClient::HandleConnect(const boost::system::error_code& err)
		{
			if(!err)
			{
				EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "[EVL_NET] connected to target endpoint succeed.");
				session_ = new TCPSession(storage_impl_->io_service_,
					&storage_impl_->socket_,
					storage_impl_->on_data_received_handler_,
					storage_impl_->on_data_written_handler_,
					storage_impl_->on_error_from_server_handler_);
				session_->set_tcp_client(this);
				session_->set_user_data(this);
				session_->Start();
			}
			else
			{
				session_ = NULL;
			}

			storage_impl_->on_connected_to_server_handler_(err);
		}

		void TCPClient::SendMsg(const void* msg, size_t msg_length, bool delay_send /* = false */)
		{
			if(is_valid())
			{
				session_->SendMsg(msg, msg_length, delay_send);
			}
			else
			{
				EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "[EVL_NET] session invalid.");
			}
		}

		void TCPClient::SendMsg(const void* msg, size_t msg_length, UserDefinedDataCopyHandlerType copy_handler)
		{
			if(is_valid())
			{
				session_->SendMsg(msg, msg_length, copy_handler);
			}
			else
			{
				EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "[EVL_NET] session invalid.");
			}
		}



	} // namespace net
} // namespace evl
