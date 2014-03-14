#include "Http_Server.hpp"
#include "../Vod/Vod_Server.hpp"

#include <sstream>

namespace Http
{
    Server::Server ( Vod::Server * server ) : mpvodServer ( server ) { }

    Server::~Server ( )
    {
        for ( const auto & prequest : mrequests )
        {
            delete prequest;
        }
    }

    void Server::generateCatalog ( )
    {
        std::stringstream catalog;
        catalog << "HTTP/1.1 200 OK\r\n";
        catalog << "Content-Type: text/plain; charset=utf-8\r\n";
        catalog << "Content-Length: ";

        std::stringstream body;
        body << "ServerAddress: " << maddress << "\r\n";
        body << "ServerPort: " << mport << "\r\n";

        for ( const auto & pentry : mpvodServer -> mcatalogEntries )
        {
            body << * pentry;
        }
        body << "\r\n";

        catalog << body.str ( ).length ( ) << "\r\n";
        catalog << "\r\n";
        catalog << body.str ( );

        mcatalog = catalog.str ( );
    }


    void Server::readEventAction ( )
    {
        CatalogRequest * request = accept <CatalogRequest> ( );
        if ( request )
        {
            request -> mpserver = this;
            mrequests.insert ( request );
            notifier -> subscribe ( request );
        }
    }

    void Server::writeEventAction ( ) { }
    void Server::exceptEventAction ( ) { }

    void Server::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        event.setRead ( );
    }
}
