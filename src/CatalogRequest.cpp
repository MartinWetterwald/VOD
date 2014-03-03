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

    // Send response here
    delete this;
}

void CatalogRequest::exceptEventAction ( )
{
    std::cout << "[HTTP] Unexpected client exception: killed" << std::endl;
    delete this;
}
