#include <iostream>
#include <sstream>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include "evl_net/tcp_server.h"
#include <third_party/utility/data_dump.hpp>
#include "evl_net/detail/net_mgr.h"

using namespace evl::net;

void OnNewClientConnectedHandler(evl::net::TCPSession* session)
{
	EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "new client connected: 0x" << session);
}

void OnDataReceivedHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{
	EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "data received from client: 0x" << session);
}

void OnDataWrittenHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{

}

void OnErrorFromClientHandler(evl::net::TCPSession* session, const boost::system::error_code& err)
{
	EVL_LOG_DEBUG(sNetMgr.get_evl_logger(), "client with error: 0x" << session << " : " << err.message());
}

boost::asio::io_service g_io_service;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: ./test_server <port number>" << std::endl;
        return 1;
    }


    short port = 0;
    std::stringstream ss; ss << argv[1]; ss >> port;
	evl::net::TCPServer server(g_io_service, port, OnNewClientConnectedHandler, OnDataReceivedHandler, OnDataWrittenHandler, OnErrorFromClientHandler);
	server.StartAccept();
	g_io_service.run();

	while(true)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
	}
	system("PAUSE");
	return 0;
}

