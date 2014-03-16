#include "Vod_TcpPull_ControlConnection.hpp"
#include "Vod_TcpPull_Server.hpp"

#include <iostream>
#include <cstring>

namespace Vod {
namespace TcpPull
{
    ControlConnection::ControlConnection ( int sock, const NetFlux::Net::InetAddress & address )
        : Request ( sock, address ), mpserver ( nullptr ), mcursor ( 0 )
    {
        std::cout << * this << " : connection established" << std::endl;
    }

    ControlConnection::~ControlConnection ( )
    {
        if ( mpserver )
        {
            mpserver -> mstreams.erase ( this );
        }
    }

    bool ControlConnection::requestEventAction ( )
    {
        return true;
    }

    bool ControlConnection::writeEventAction ( )
    {
        return true;
    }

    bool ControlConnection::exceptEventAction ( )
    {
        std::cout << * this << " : unexpected exception -> killed" << std::endl;
        delete this;
        return false;
    }

    bool ControlConnection::timeoutEventAction ( )
    {
        std::cout << * this << " : hasn't talked for a long time -> killed" << std::endl;
        delete this;
        return false;
    }

    void ControlConnection::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        event.setRead ( );
        event.setTimeout ( 5000000 );
    }

    void ControlConnection::toString ( std::ostream & os ) const
    {
        os << "TcpPull socket ";
        NetFlux::Tcp::ServerStream::toString ( os );
    }
} }
