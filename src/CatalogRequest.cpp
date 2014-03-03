#include "CatalogRequest.hpp"
#include "HttpServer.hpp"

#include <iostream>
#include <cstring>

CatalogRequest::CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address )
    : Request ( sock, address ), mpserver ( nullptr ), mcursor ( 0 ) { }

CatalogRequest::~CatalogRequest ( )
{
    if ( mpserver )
    {
        mpserver -> mrequests.erase ( this );
    }
}

void CatalogRequest::writeEventAction ( )
{
    if ( reading )
    {
        delete this;
        return;
    }

    // TODO: send catalog.
    std::cout << * this << " : Catalog sent." << std::endl;
    delete this;
}

void CatalogRequest::exceptEventAction ( )
{
    std::cout << * this << " : unexpected exception -> killed" << std::endl;
    delete this;
}

void CatalogRequest::requestEventAction ( )
{
    reading = false;
}
