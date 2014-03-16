#include "Vod_TcpPull_Stream.hpp"
#include "Vod_TcpPull_Server.hpp"

#include <iostream>
#include <cstring>

namespace Vod {
namespace TcpPull
{
    Stream::Stream ( int sock, const NetFlux::Net::InetAddress & address )
        : NetFlux::Tcp::ServerStream ( sock, address ), mpserver ( nullptr ), mcursor ( 0 )
    {
        std::cout << * this << " : connection established" << std::endl;
    }

    Stream::~Stream ( )
    {
        if ( mpserver )
        {
            mpserver -> mstreams.erase ( this );
        }
    }

    bool Stream::writeEventAction ( )
    {
        return true;
    }

    bool Stream::exceptEventAction ( )
    {
        std::cout << * this << " : unexpected exception -> killed" << std::endl;
        delete this;
        return false;
    }

    bool Stream::timeoutEventAction ( )
    {
        std::cout << * this << " : hasn't talked for a long time -> killed" << std::endl;
        delete this;
        return false;
    }

    void Stream::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        event.setRead ( );
        event.setTimeout ( 5000000 );
    }

    bool Stream::readEventAction ( )
    {
        std::cout << "Coucou" << std::endl;
        return true;
    }

    void Stream::toString ( std::ostream & os ) const
    {
        os << "TcpPull socket ";
        NetFlux::Tcp::ServerStream::toString ( os );
    }
} }
