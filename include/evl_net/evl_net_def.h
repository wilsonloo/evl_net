#ifndef EVL_NET_DEF_H_
#define EVL_NET_DEF_H_

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>
#include "config.h"

namespace evl
{
	namespace net
	{
		enum
		{
			// 最大读取写入数据长度
			MAX_DATA_LENGTH_PER_PACKET = 1024,
			MAX_DATA_WRITTING_LEN_PER_PACKET = MAX_DATA_LENGTH_PER_PACKET,

			// 为防止接收数据过多，此处设置为 比 write 空间大
			MAX_DATA_READING_LEN_PER_PACKET = 2 * MAX_DATA_WRITTING_LEN_PER_PACKET,
		};

		// 唯一ID定义
		typedef std::size_t UUIDType;
		const UUIDType UUID_MIN_ID = 1;
		const UUIDType UUID_MAX_ID = 0xFFFFfffe;
		const UUIDType UUID_INVALID_ID = 0xFFFFffff;

		class EVL_NET_DECL TCPSession;
		typedef boost::function<void (TCPSession* session, const char* data, size_t bytes_transfered)> OnDataReceivedHandlerType;
		typedef boost::function<void (TCPSession* session, const char* data, size_t bytes_transfered)> OnDataWrittenHandlerType;
		typedef boost::function<void (TCPSession* session, const boost::system::error_code& err)> OnErrorHandlerType;

		typedef boost::function<void (TCPSession* new_server)> OnNewClientConnectedHandlerType;
		typedef boost::function<void (const boost::system::error_code& err)> OnConnectedToServerHandlerType;
	} // namespace net
} // namespace evl

#endif // EVL_NET_DEF_H_