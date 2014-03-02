#include "HttpRequest.hpp"
#include "HttpServer.hpp"
#include <iostream>


HttpRequest::HttpRequest ( int sock, const NetFlux::Net::InetAddress & address ) :
    NetFlux::Tcp::ServerStream ( sock, address ),
    reading ( true ),
    mpserver ( nullptr )
{
    std::cout << "[HTTP] Client accepted" << std::endl;
}

HttpRequest::~HttpRequest ( )
{
    if ( mpserver )
    {
        mpserver -> mrequests.erase ( this );
    }
}

void HttpRequest::readEventAction ( )
{
    char buffer [ 1024 ];
    ssize_t ret = recv ( buffer, 1024 );

    if ( ret == 0 )
    {
        std::cout << "[HTTP] Client closed connection" << std::endl;
        delete this;
        return;
    }

    std::cout << "[HTTP] Got " << ret << " bytes from client" << std::endl;
}

void HttpRequest::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
{
    if ( reading )
    {
        event.setRead ( );
        return;
    }

    event.setWrite ( );
}
