#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "../Http/HttpServer.hpp"

class VODServer
{
public:
    VODServer ( );
    virtual ~VODServer ( );

    bool start ( );

private:
    bool parseStartUpFile ( );
    bool parseStream ( const std::string & path );

    NetFlux::SocketIOEvent::Notifier * mpNotifier;
    HttpServer * mpHttpServer;
};

#endif
