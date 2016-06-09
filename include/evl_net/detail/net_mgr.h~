#ifndef EVL_ENGINE_EVL_NET_NET_MGR_H_
#define EVL_ENGINE_EVL_NET_NET_MGR_H_

#include <boost/asio.hpp>
#include <third_party/utility/singleton.hpp>
#include <evl_logger/evl_logger.h>
#include "../config.h"

namespace evl
{
	namespace net
	{
		class EVL_NET_DECL NetMgr : public evl::utility::Singleton<NetMgr>
		{
			friend class evl::utility::Singleton<NetMgr>;

		public:
			NetMgr();
			virtual ~NetMgr();

			inline evl::utility::EvlLogger& get_evl_logger(){ return logger_; }

			boost::asio::io_service& get_ios(){ return ios_; }

		public:
			evl::utility::EvlLogger logger_;
			boost::asio::io_service ios_;
		};

		EVL_NET_DECL NetMgr& get_instance();
	}
}

#define sNetMgr evl::net::NetMgr::get_mutable_instance()

#endif // EVL_ENGINE_EVL_NET_NET_MGR_H_
