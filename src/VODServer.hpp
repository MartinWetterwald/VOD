#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "HttpServer.hpp"

class VODServer
{
public:
    VODServer ( );
    virtual ~VODServer ( );

    bool start ( );

private:
    bool parseStartUpFile ( );

    NetFlux::SocketIOEvent::Notifier * mpNotifier;
    HttpServer * mpHttpServer;
};

#endif
