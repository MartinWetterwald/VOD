#ifndef _CATALOG_REQUEST_HPP_
#define _CATALOG_REQUEST_HPP_

#include "../Vod/Vod_Request.hpp"

namespace Http
{
    class Server;

    class CatalogRequest : public Vod::Request
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
        CatalogRequest ( const CatalogRequest & ) = delete;
        CatalogRequest & operator= ( const CatalogRequest & ) = delete;


        friend class Server;
    };
}

#endif

