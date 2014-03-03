#include "HttpRequest.hpp"
#include "HttpServer.hpp"
#include <iostream>


HttpRequest::HttpRequest ( int sock, const NetFlux::Net::InetAddress & address ) :
    NetFlux::Tcp::ServerStream ( sock, address ),
    reading ( true ),
    mpserver ( nullptr ),
    endRequest ( 0 )
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
    if ( ! reading )
    {
        std::cout << "[HTTP] Client talked during response: killed" << std::endl;
        delete this;
        return;
    }

    char buffer [ 1024 ];
    ssize_t ret = recv ( buffer, 1024 );

    if ( ret == -1 )
    {
        std::cout << "[HTTP] Client receive error: killed" << std::endl;
        delete this;
        return;
    }

    if ( ret == 0 )
    {
        std::cout << "[HTTP] Client closed connection" << std::endl;
        delete this;
        return;
    }

    std::cout << "[HTTP] Got " << ret << " bytes from client" << std::endl;

    ssize_t pos = 0;
    for ( char * cursor = buffer ; pos < ret ; ++cursor, ++pos )
    {
        if ( endRequest % 2 == 0 && * cursor == '\r' )
        {
            endRequest++;
        }
        else if ( endRequest % 2 == 1 && * cursor == '\n' )
        {
            endRequest++;
            if ( endRequest == 4 )
            {
                reading = false;
            }
        }
        else
        {
            endRequest = 0;
        }
    }
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
