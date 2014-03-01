#include <SocketIOEvent/Notifier.hpp>
#include <iostream>

#include "VODServer.hpp"

int main ( )
{
    NetFlux::SocketIOEvent::Notifier notifier;
    VODServer vodServer;
    if ( ! vodServer.start ( 8080, &notifier ) )
    {
        std::cout << "An error occured while starting the VOD Server."  << std::endl;
        return -1;

    }
    notifier.startNotify ( );
    return 0;
}
