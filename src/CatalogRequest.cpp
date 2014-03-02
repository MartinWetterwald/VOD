#include "CatalogRequest.hpp"

CatalogRequest::CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address )
    : HttpRequest ( sock, address ) { }

CatalogRequest::~CatalogRequest ( ) { }

void CatalogRequest::writeEventAction ( )
{
    delete this;
}

void CatalogRequest::exceptEventAction ( )
{
    delete this;
}
