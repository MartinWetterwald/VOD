#include "Vod_TcpPull_Server.hpp"
#include "../Vod_Server.hpp"

#include <iostream>

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

    void Server::toString ( std::ostream & os ) const
    {
        os << "TcpPull Server (" << mname << ")";
    }

    bool Server::start ( )
    {
        if ( listen ( CatalogEntry::mport ) )
        {
            mpvodServer -> mpNotifier -> subscribe ( this );

            std::cout << "TcpPull Server listening on ";
            NetFlux::Tcp::Server::toString ( std::cout );
            std::cout << std::endl;

            return true;
        }

        return false;
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
