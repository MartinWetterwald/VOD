#include "Vod_TcpClient.hpp"
#include <iostream>

namespace Vod
{
    TcpClient::TcpClient ( ) : writing ( false )
    {
    }

    TcpClient::~TcpClient ( )
    {
    }

    bool TcpClient::readEventAction ( )
    {
        std::cout << "Read Event" << std::endl;
        return true;
    }

    bool TcpClient::writeEventAction ( )
    {
        writing = false;
        return true;
    }

    bool TcpClient::exceptEventAction ( )
    {
        std::cout << "Except Event" << std::endl;
        return true;
    }

    bool TcpClient::timeoutEventAction ( )
    {
        std::cout << "Timeout Event" << std::endl;
        return true;
    }

    void TcpClient::chooseSubscription ( NetFlux::SocketIOEvent::Event & e )
    {
        if ( writing )
        {
            e.setWrite ( );
        }
    }
}
