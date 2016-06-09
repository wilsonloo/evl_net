#include "evl_net/tcp_client.h"
#include <iostream>
#include <sstream>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>

void OnConnectedToServerHandler(const boost::system::error_code& err)
{
    std::cout << "connected to remote server " << std::endl;
}

void OnDataReceivedHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{
    std::cout << "data received from server " << std::endl;
}

void OnDataWrittenHandler(evl::net::TCPSession* session, const char* data, size_t bytes_transfered)
{
    std::cout << "data written to server " << std::endl;
}

void OnErrorFromServerHandler(evl::net::TCPSession* session, const boost::system::error_code& err)
{
    std::cout << "error from server connection" << std::endl;
}

boost::asio::io_service g_io_service;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: ./test_client <port number>" << std::endl;
        return 1;
    }

    short port = 0;
    std::stringstream ss; ss << argv[1]; ss >> port;

	evl::net::TCPClient client(g_io_service, OnConnectedToServerHandler, 
		OnDataReceivedHandler, 
		OnDataWrittenHandler, 
		OnErrorFromServerHandler);

    std::cout << "connecting to " << port << " ..." << std::endl;
	client.StartConnect("localhost", port);

	boost::thread t(boost::bind(&boost::asio::io_service::run, &g_io_service));

	while(true)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
	}
	system("PAUSE");
	return 0;
}

