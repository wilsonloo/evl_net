#include "evl_net/tcp_session.h"
#include "evl_net/detail/net_mgr.h"
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <boost/scope_exit.hpp>
#include <third_party/utility/data_dump.hpp>
#include "tcp_session_storage_impl.h"

namespace evl
{
	namespace net
	{
		TCPSession::TCPSession(boost::asio::io_service& io_service, 
			OnDataReceivedHandlerType on_data_received_handler, 
			OnDataWrittenHandlerType on_data_written_handler, 
			OnErrorHandlerType on_error_from_client_handler)
			: io_service_(io_service)
			, socket_(NULL)
			, socket_self_created_(false)
			, storage_impl_(NULL)
			, user_data_(NULL)
			, writing_data_(NULL)
			, uuid_(UUID_INVALID_ID)
			, connected_(false)
			, closed_(true)
		{
			BOOST_ASSERT(on_data_received_handler != 0);
			BOOST_ASSERT(on_data_written_handler != 0);

			storage_impl_ = new TCPSessionStorageImpl(on_data_received_handler, on_data_written_handler, on_error_from_client_handler);
			socket_ = new boost::asio::ip::tcp::socket(io_service);
			socket_self_created_ = true;
			writing_data_ = new evl::utility::ReadWriteArray<MAX_DATA_LENGTH_PER_PACKET * 6>;
		}

		TCPSession::TCPSession(boost::asio::io_service& io_service, 
			boost::asio::ip::tcp::socket* socket,
			OnDataReceivedHandlerType on_data_received_handler, 
			OnDataWrittenHandlerType on_data_written_handler, 
			OnErrorHandlerType on_error_from_client_handler)
			: io_service_(io_service)
			, socket_(socket)
			, socket_self_created_(false)
			, storage_impl_(NULL)
			, user_data_(NULL)
			, writing_data_(NULL)
			, uuid_(UUID_INVALID_ID)
			, connected_(false)
			, closed_(true)
		{
			BOOST_ASSERT(on_data_received_handler != 0);
			BOOST_ASSERT(on_data_written_handler != 0);

			storage_impl_ = new TCPSessionStorageImpl(on_data_received_handler, on_data_written_handler, on_error_from_client_handler);
			writing_data_ = new evl::utility::ReadWriteArray<MAX_DATA_LENGTH_PER_PACKET * 6>;
		}

		TCPSession::~TCPSession()
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "TCPSession::~TCPSession() ...");
			if(storage_impl_ != NULL)
			{
				delete storage_impl_;
				storage_impl_ = NULL;
			}

			if(socket_self_created_ && socket_ != NULL)
			{
				delete socket_;
				socket_ = NULL;
				socket_self_created_ = false;
			}

			if(writing_data_ != NULL)
			{
				delete writing_data_;
				writing_data_ = NULL;
			}

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "TCPSession::~TCPSession() ...Done");
		}

		void TCPSession::Start()
		{
			connected_ = true;
			closed_ = false;

			EVL_LOG_DEBUG_FUNCLINE (sNetMgr.get_evl_logger(), "[EVL_NET]client session:" << this << " working...");

			socket_->async_read_some(
				boost::asio::buffer(data_), 
				boost::bind(&TCPSession::HandleRead, this,
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred));
		}

		void TCPSession::HandleRead(const boost::system::error_code& err, 
									size_t bytes_transfered)
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET] client session received... ");

			if(err)
			{
				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET] client session received... Failed:" << err.message());

				Close();
				return;
			}

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET] client session received " << bytes_transfered << " data");
#ifdef DEBUG_DEVELOP_EVL_NET
			evl::utility::data_dump(stdout, &data_[0], bytes_transfered, "DATA-RECEIVED");
#endif // DEBUG_DEVELOP_EVL_NET

			storage_impl_->on_data_received_handler_(this, &data_[0], bytes_transfered);

			// session may be set closed on_data_received_handler_()
			if (closed())
			{
				Close();
				return;
			}

			/*
			boost::asio::async_write(*socket_, 
				boost::asio::buffer(data_, bytes_transfered), 
				boost::bind(&TCPSession::HandleWrite, this, &data_[0], bytes_transfered,
				boost::asio::placeholders::error));
			*/
			socket_->async_read_some(
				boost::asio::buffer(data_), 
				boost::bind(&TCPSession::HandleRead, this, 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred));			

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET] client session received...Done");
		}

		void TCPSession::HandleWrite(const char* data, size_t bytes_transferred, 
			const boost::system::error_code& err, size_t bytes_transfered)
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET]HandleWrite...");

			if(err)
			{
				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET]HandleWrite... Failed:" << err.message().c_str());

				Close();
				return;
			}

			// shutdown event maybe raised befown HandleWrite() event occurd
			// if so, rawly close the sockets and not send data more
			if(closed())
			{
				Close();
				return;
			}
	
			
#ifdef DEBUG_DEVELOP_EVL_NET
			evl::utility::data_dump(stdout, data, bytes_transfered, "DATA-WITTEN");
#endif // DEBUG_DEVELOP_EVL_NET

			storage_impl_->on_data_written_handler_(this, data, bytes_transfered);
			writing_data_->RemoveData(bytes_transferred);
			_send_msg();

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "[EVL_NET]HandleWrite...Done");
		}

		void TCPSession::Close()
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "closing TCPSession ...");

			closed_ = true;
			if (storage_impl_ != NULL)
			{
				boost::system::error_code err;
				storage_impl_->on_error_from_client_handler_(this, err);
			}

			if (socket_ != NULL)
			{
				socket_->close();

				// do not delete socket_, as it may be parsed outside
			}

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "closing TCPSession ...Done");
			delete this;
		}

		void TCPSession::Shutdown()
		{
			if (closed())
				return;

			closed_ = true;
		}

		void TCPSession::SendMsg(const void* msg, size_t msg_length, bool delay_send /* = false */)
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "require send msg(1)...");

			if (!connected())
				return;

			if (closed())
				return;

			if(msg_length > MAX_DATA_LENGTH_PER_PACKET)
			{
				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "sending too much data out of per packet.length: " << msg_length);

				return;
			}

			bool is_writing = !writing_data_->is_empty();
			if(!writing_data_->AddData(msg, msg_length))
			{
				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "failded to send msg, current writing not finished: " << msg_length);

				return;
			}

			// 是否延迟发送
			if (delay_send)
				return;

			// 非延迟发送、并且并没有在写， 则马上发送
			if(!is_writing)
				_send_msg();

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "require send msg(1)...Done");
		}

		void TCPSession::SendMsg(const void* msg, size_t msg_length, UserDefinedDataCopyHandlerType copy_handler)
		{
			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "require send msg(2)...");

			if (!connected())
				return;

			if (closed())
				return;

			if(copy_handler == 0)
				return;

			if(msg_length > MAX_DATA_LENGTH_PER_PACKET)
			{
				EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "sending too much data out of per packet.length: " << msg_length);

				return;
			}

			bool is_writing = !writing_data_->is_empty();
			void* dest = writing_data_->GetAndLockWritableBlock(msg_length);
			bool suceed = false;
			BOOST_SCOPE_EXIT(&suceed, &dest, &writing_data_)
			{
				if(!suceed && dest != NULL)
				{
					writing_data_->ReleaseWritableBlock(dest);
				}
			}BOOST_SCOPE_EXIT_END
			
			copy_handler(dest, msg, msg_length);
			if(dest != NULL)
			{
				writing_data_->SubmitWritableBlock(dest, msg_length);
			}

			if(!is_writing)
			{
				_send_msg();
			}
			suceed = true;

			EVL_LOG_DEBUG_FUNCLINE(sNetMgr.get_evl_logger(), "require send msg(2)...Done");
		}

		void TCPSession::_send_msg()
		{
			if(writing_data_->is_empty())
				return;

			const char* reading_data = NULL;
			size_t reading_data_length = 0;
			if(writing_data_->get_read_data(reading_data, reading_data_length))
			{
				boost::asio::async_write(*socket_, 
					boost::asio::buffer(reading_data, reading_data_length), 
					boost::bind(&TCPSession::HandleWrite, this, reading_data, reading_data_length,
						boost::asio::placeholders::error, 
						boost::asio::placeholders::bytes_transferred));
			}
		}

		void TCPSession::Flush()
		{
			if (!connected())
				return;

			if (closed())
				return;

			_send_msg();
		}
	}
}
