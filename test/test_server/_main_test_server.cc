#ifdef TEST_SERVER

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include "include/tcp_server.h"
#include <third_party/utility/include/data_dump.hpp>
#include "packet_coding_xor.h"
#include "net_mgr.h"

using namespace evl::net;

void OnNewClientConnectedHandler(evl::net::TCPSession* session)
{
	EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "new client connected: 0x" << session);
}

void OnDataReceivedHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{
	char buff[4 + MAX_DATA_READING_LEN_PER_PACKET + 1] = { 0, };
	memcpy(buff, data, bytes_transfered);

	PacketCodingXor coder;
	coder.Decrypt(&buff[0] + 4, bytes_transfered - 4);

	EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "data received from client: 0x" << session);
	evl::utility::data_dump(stdout, buff, bytes_transfered);
}

void OnDataWrittenHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{

}

void OnErrorFromClientHandler(evl::net::TCPSession* session, const boost::system::error_code& err)
{
	EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "client with error: 0x" << session << " : " << err.message());
}

boost::asio::io_service g_io_service;

int main()
{
	evl::net::TCPServer server(g_io_service, 29999, OnNewClientConnectedHandler, OnDataReceivedHandler, OnDataWrittenHandler, OnErrorFromClientHandler);
	server.StartAccept();
	g_io_service.run();

	while(true)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
	}
	system("PAUSE");
	return 0;
}

#endif // TEST_SERVER