#include "Request.hpp"
#include "HttpServer.hpp"

HttpRequest::HttpRequest ( int sock, const NetFlux::Net::InetAddress & address )
    : Request ( sock, address ), mpserver ( nullptr ) { }

HttpRequest::~HttpRequest ( )
{
    if ( mpserver )
    {
        mpserver -> mrequests.erase ( this );
    }
}

void HttpRequest::requestEventAction ( )
{
    reading = false;
}
