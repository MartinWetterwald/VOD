#ifndef _CATALOG_REQUEST_HPP_
#define _CATALOG_REQUEST_HPP_

#include "HttpRequest.hpp"

class CatalogRequest : public HttpRequest
{
public:
    CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address );
    ~CatalogRequest ( );

protected:
    virtual void writeEventAction ( );
    virtual void exceptEventAction ( );

private:
    CatalogRequest ( ) = delete;
};

#endif

