#ifndef _HTTP_SERVER_HPP_
#define _HTTP_SERVER_HPP_

#include <Tcp/Tcp_Server.hpp>
#include <unordered_set>

#include "Http_CatalogRequest.hpp"

namespace Vod
{
    class Server;
}

namespace Http
{
    class Server : public NetFlux::Tcp::Server
    {
    public:
        Server ( Vod::Server * server );
        virtual ~Server ( );

        inline bool listen ( )
        {
            return NetFlux::Tcp::Server::listen ( mport );
        }

        void generateCatalog ( );

    protected:
        virtual void readEventAction ( );
        virtual void writeEventAction ( );
        virtual void exceptEventAction ( );

        virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );


    private:
        std::unordered_set <CatalogRequest *> mrequests;
        Vod::Server * mpvodServer;

        Server ( const Server & ) = delete;
        Server & operator= ( const Server & ) = delete;

        std::string mcatalog;
        std::string maddress;
        uint16_t mport;


        friend class CatalogRequest;
        friend class Vod::Server;
    };
}

#endif

