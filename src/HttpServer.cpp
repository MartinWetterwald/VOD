#include "HttpServer.hpp"

HttpServer::~HttpServer ( )
{
    for ( const auto & prequest : mrequests )
    {
        prequest -> mpserver = nullptr;
        delete prequest;
    }
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
