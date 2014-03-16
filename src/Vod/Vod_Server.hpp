#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "../Http/Http_Server.hpp"
#include "Vod_CatalogEntry.hpp"

#include <map>

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
        bool parseImage ( std::string & path );

        Server ( const Server & ) = delete;
        Server & operator= ( const Server & ) = delete;

        NetFlux::SocketIOEvent::Notifier * mpNotifier;
        Http::Server * mpHttpServer;

        std::map <uint32_t, CatalogEntry *> mcatalogEntries;



        friend class Http::Server;
    };
}

#endif
