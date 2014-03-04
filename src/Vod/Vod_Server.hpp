#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "../Http/Http_Server.hpp"

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

        inline std::istream & customGetLine ( std::istream & is, std::string & str );

        NetFlux::SocketIOEvent::Notifier * mpNotifier;
        Http::Server * mpHttpServer;
    };
}

#endif
