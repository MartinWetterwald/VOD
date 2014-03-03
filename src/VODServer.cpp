#include "VODServer.hpp"

#include <SocketIOEvent/Notifier.hpp>
#include <iostream>

VODServer::VODServer ( )
{
    mpNotifier = new NetFlux::SocketIOEvent::Notifier;
    mpHttpServer = new HttpServer;
}

VODServer::~VODServer ( )
{
    delete mpHttpServer;
    delete mpNotifier;
}

bool VODServer::start ( uint16_t port )
{
    if ( ! mpHttpServer -> listen ( port ) )
    {
        return false;
    }
    std::cout << "HTTP Server listening on " << * mpHttpServer << std::endl;

    mpNotifier -> subscribe ( mpHttpServer );

    mpNotifier -> startNotify ( );
    return true;
}
