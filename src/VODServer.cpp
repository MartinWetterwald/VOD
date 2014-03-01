#include "VODServer.hpp"

void VODServer::start ( )
{
    httpServer = new HTTPServer ( );
}

VODServer::~VODServer ( )
{
    if ( httpServer )
    {
        delete httpServer;
    }
}
