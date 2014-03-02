#include "HTTPServer.hpp"

HTTPServer::~HTTPServer ( )
{
    for ( const auto & prequest : mrequests )
    {
        prequest -> mpserver = nullptr;
        delete prequest;
    }
}

void HTTPServer::readEventAction ( )
{
    CatalogRequest * request = accept <CatalogRequest> ( );
    if ( request )
    {
        request -> mpserver = this;
        mrequests.insert ( request );
        notifier -> subscribe ( request );
    }
}

void HTTPServer::writeEventAction ( ) { }
void HTTPServer::exceptEventAction ( ) { }

void HTTPServer::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
{
    event.setRead ( );
}
