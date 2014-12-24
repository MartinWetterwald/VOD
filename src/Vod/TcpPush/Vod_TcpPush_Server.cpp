#include "Vod_TcpPush_Server.hpp"
#include "../Vod_Server.hpp"

#include <iostream>

namespace Vod {
namespace TcpPush
{
    Server::~Server ( )
    {
        for ( const auto & pstream : mstreams )
        {
            pstream -> mpserver = nullptr;
            delete pstream;
        }
    }

    void Server::toTitleString ( std::ostream & os ) const
    {
        os << "TcpPush Server (";
        CatalogEntry::toTitleString ( os );
        os << ")";
    }

    bool Server::start ( )
    {
        if ( listen ( CatalogEntry::mport ) )
        {
            mpvodServer -> mpNotifier -> subscribe ( this );

            std::cout
                << * static_cast <CatalogEntry *> ( this )
                << " listening on "
                << * static_cast <NetFlux::Tcp::Server *> ( this )
                << std::endl;

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

            std::cout << * stream << " : connection established" << std::endl;

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
