#ifndef _CATALOG_REQUEST_HPP_
#define _CATALOG_REQUEST_HPP_

#include "../Vod/Request.hpp"

namespace Http
{
    class Server;
}

class CatalogRequest : public Request
{
public:
    CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address );
    ~CatalogRequest ( );

protected:
    virtual void requestEventAction ( );
    virtual void writeEventAction ( );
    virtual void exceptEventAction ( );

    void toString ( std::ostream & os ) const;

    Http::Server * mpserver;
    uint32_t mcursor;

private:
    CatalogRequest ( ) = delete;



    friend class Http::Server;
};

#endif

