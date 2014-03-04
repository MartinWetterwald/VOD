#include "CatalogRequest.hpp"
#include "HttpServer.hpp"

#include <iostream>
#include <cstring>

CatalogRequest::CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address )
    : Request ( sock, address ), mpserver ( nullptr ), mcursor ( 0 )
{
    std::cout << * this << " : connection established" << std::endl;
}

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

    const char * data = mpserver -> mcatalog.c_str ( );
    size_t length = mpserver -> mcatalog.length ( );

    ssize_t sent = send ( data + mcursor, length - mcursor );

    if ( sent == -1 )
    {
        std::cout << * this << " : send failed -> killed" << std::endl;
        delete this;
        return;
    }

    mcursor += sent;

    if ( mcursor >= length )
    {
        std::cout << * this << " : catalog sent (" << length << " bytes) -> closing connection" << std::endl;
        delete this;
    }
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

void CatalogRequest::toString ( std::ostream & os ) const
{
    os << "Catalog Request socket ";
    NetFlux::Tcp::ServerStream::toString ( os );
}
