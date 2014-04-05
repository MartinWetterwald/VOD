#include "Vod_TcpPull_Server.hpp"

namespace Vod {
namespace TcpPull
{
    Server::~Server ( )
    {
        for ( const auto & pstream : mstreams )
        {
            delete pstream;
        }
    }

    bool Server::readEventAction ( )
    {
        ControlConnection * stream = accept <ControlConnection> ( );
        if ( stream )
        {
            stream -> mpserver = this;
            mstreams.insert ( stream );
            notifier -> subscribe ( stream );
        }

        return true;
    }

    bool Server::writeEventAction ( ) { return true; }
    bool Server::exceptEventAction ( ) { return true; }

    void Server::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        event.setRead ( );
    }
} }
