#include "VODServer.hpp"

#include <SocketIOEvent/Notifier.hpp>

VODServer::VODServer ( )
{
    mpHttpServer = new HTTPServer ( );
}

bool VODServer::start ( uint16_t port, NetFlux::SocketIOEvent::Notifier * notif )
{
    if ( ! mpHttpServer -> listen ( port ) )
    {
        return false;
    }

    notif -> subscribe ( mpHttpServer );
    return true;
}

VODServer::~VODServer ( )
{
    delete mpHttpServer;
}
