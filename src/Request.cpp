#include "Request.hpp"
#include <iostream>


Request::Request ( int sock, const NetFlux::Net::InetAddress & address ) :
    NetFlux::Tcp::ServerStream ( sock, address ),
    reading ( true ),
    endRequest ( 0 )
{
    std::cout << * this << " : connection established" << std::endl;
}

Request::~Request ( ) { }

void Request::readEventAction ( )
{
    if ( ! reading )
    {
        std::cout << * this << " : talked during response -> killed" << std::endl;
        delete this;
        return;
    }

    char buffer [ 1024 ];
    ssize_t ret = recv ( buffer, 1024 );

    if ( ret == -1 )
    {
        std::cout << * this << " : receive error -> killed" << std::endl;
        delete this;
        return;
    }

    if ( ret == 0 )
    {
        std::cout << * this << " : client closed connection" << std::endl;
        delete this;
        return;
    }

    std::cout << * this << " : got " << ret << " bytes from him" << std::endl;

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
                requestEventAction ( );
            }
        }
        else
        {
            endRequest = 0;
        }
    }
}

void Request::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
{
    if ( reading )
    {
        event.setRead ( );
        return;
    }

    event.setWrite ( );
}

void Request::toString ( std::ostream & os ) const
{
    os << "Request socket ";
    NetFlux::Tcp::ServerStream::toString ( os );
}
