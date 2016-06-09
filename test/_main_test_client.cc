#ifdef TEST_CLIENT

#include "include/tcp_client.h"
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>

void OnConnectedToServerHandler(evl::net::TCPSession* session, const boost::system::error_code& err)
{

}
void OnDataReceivedHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{

}

void OnDataWrittenHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{

}

void OnErrorFromServerHandler(evl::net::TCPSession* session, const boost::system::error_code& err)
{

}

boost::asio::io_service g_io_service;

int main()
{
	evl::net::TCPClient client(g_io_service, OnConnectedToServerHandler, 
		OnDataReceivedHandler, 
		OnDataWrittenHandler, 
		OnErrorFromServerHandler);

	client.StartConnect("localhost", 29999);

	boost::thread t(boost::bind(&boost::asio::io_service::run, &g_io_service));

	while(true)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
	}
	system("PAUSE");
	return 0;
}

#endif // TEST_SERVER