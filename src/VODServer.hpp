#ifndef _VOD_SERVER_HPP_
#define _VOD_SERVER_HPP_

#include "HTTPServer.hpp"

class VODServer
{
public:
    VODServer ( ) = default;
    virtual ~VODServer ( );

    void start ( );

protected:
    HTTPServer * httpServer;
};

#endif
