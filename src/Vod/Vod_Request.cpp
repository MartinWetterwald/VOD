#include "Vod_Request.hpp"
#include <iostream>


namespace Vod
{
    Request::Request ( int sock, const NetFlux::Net::InetAddress & address ) :
        NetFlux::Tcp::ServerStream ( sock, address ),
        reading ( true ),
        endRequest ( 0 ) { }

    Request::~Request ( ) { }

    bool Request::readEventAction ( )
    {
        if ( ! reading )
        {
            std::cout << * this << " : talked during response -> killed" << std::endl;
            delete this;
            return false;
        }

        char buffer [ 1024 ];
        ssize_t ret = recv ( buffer, 1024 );

        if ( ret == -1 )
        {
            std::cout << * this << " : receive error -> killed" << std::endl;
            delete this;
            return false;
        }

        if ( ret == 0 )
        {
            std::cout << * this << " : foreign host closed connection" << std::endl;
            delete this;
            return false;
        }

        std::cout << * this << " : got " << ret << " bytes from foreign host" << std::endl;

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

        return true;
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
}
