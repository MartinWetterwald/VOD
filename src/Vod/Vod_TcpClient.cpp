#include "Vod_TcpClient.hpp"

namespace Vod
{
    TcpClient::TcpClient ( )
    {
    }

    TcpClient::~TcpClient ( )
    {
    }

    bool TcpClient::readEventAction ( )
    {
        return readEventMapping ( this );
    }

    bool TcpClient::writeEventAction ( )
    {
        return writeEventMapping ( this );
    }

    bool TcpClient::exceptEventAction ( )
    {
        return exceptEventMapping ( this );
    }

    bool TcpClient::timeoutEventAction ( )
    {
        return timeoutEventMapping ( this );
    }

    void TcpClient::chooseSubscription ( NetFlux::SocketIOEvent::Event & e )
    {
        chooseSubscriptionMapping ( this, e );
    }
}
