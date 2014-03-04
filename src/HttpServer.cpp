#include "HttpServer.hpp"
#include <sstream>

HttpServer::~HttpServer ( )
{
    for ( const auto & prequest : mrequests )
    {
        prequest -> mpserver = nullptr;
        delete prequest;
    }
}

void HttpServer::generateCatalog ( )
{
    std::stringstream catalog;
    catalog << "HTTP/1.1 200 OK\r\n";
    catalog << "Content-Type: text/plain; charset=utf-8\r\n";
    catalog << "Content-Length: ";

    std::stringstream body;
    body << "ServerAddress: " << maddress << "\r\n";
    body << "ServerPort: " << mport << "\r\n";

    catalog << body.str ( ).length ( ) << "\r\n";
    catalog << "\r\n";
    catalog << body.str ( );

    mcatalog = catalog.str ( );
}


void HttpServer::readEventAction ( )
{
    CatalogRequest * request = accept <CatalogRequest> ( );
    if ( request )
    {
        request -> mpserver = this;
        mrequests.insert ( request );
        notifier -> subscribe ( request );
    }
}

void HttpServer::writeEventAction ( ) { }
void HttpServer::exceptEventAction ( ) { }

void HttpServer::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
{
    event.setRead ( );
}
