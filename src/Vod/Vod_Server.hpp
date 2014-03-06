#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "../Http/Http_Server.hpp"
#include "Vod_CatalogEntry.hpp"

#include <vector>

namespace Vod
{
    class Server
    {
    public:
        Server ( );
        virtual ~Server ( );

        bool start ( );

    private:
        bool parseStartUpFile ( );
        bool parseStream ( const std::string & path );
        bool parseImage ( std::string & path );
        inline std::istream & customGetLine ( std::istream & is, std::string & str );

        Server ( const Server & ) = delete;
        Server & operator= ( const Server & ) = delete;

        NetFlux::SocketIOEvent::Notifier * mpNotifier;
        Http::Server * mpHttpServer;

        std::vector <CatalogEntry *> mcatalogEntries;



        friend class Http::Server;
    };
}

#endif
