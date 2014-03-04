#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "../Http/HttpServer.hpp"

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

        NetFlux::SocketIOEvent::Notifier * mpNotifier;
        HttpServer * mpHttpServer;
    };
}

#endif
