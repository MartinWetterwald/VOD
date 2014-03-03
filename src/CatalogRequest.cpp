#include "CatalogRequest.hpp"
#include <iostream>
#include <cstring>

CatalogRequest::CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address )
    : HttpRequest ( sock, address ) { }

CatalogRequest::~CatalogRequest ( ) { }

void CatalogRequest::writeEventAction ( )
{
    if ( reading )
    {
        delete this;
        return;
    }

    // TODO: call answer
    std::cout << * this << " : Catalog sent." << std::endl;
    delete this;
}

void CatalogRequest::exceptEventAction ( )
{
    std::cout << * this << " : unexpected exception -> killed" << std::endl;
    delete this;
}
