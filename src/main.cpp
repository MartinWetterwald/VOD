#include <SocketIOEvent/SocketIOEvent_Notifier.hpp>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <csignal>
#include <cstring>
#include <cstdlib>

#include "Vod/Vod_Server.hpp"

#ifndef _WIN32
static void __attribute__ ( ( noreturn ) ) intHandler ( int sig );
#endif

static Vod::Server * vodServer = 0;

int main ( )
{
#ifndef _WIN32
    struct sigaction act;
    memset ( &act, 0, sizeof ( act ) );
    act.sa_handler = intHandler;
    sigaction ( SIGINT, &act, 0 );
#endif

    vodServer = new Vod::Server;
    if ( ! vodServer -> start ( ) )
    {
        delete vodServer;
        return EXIT_FAILURE;
    }

    delete vodServer;
    return EXIT_SUCCESS;
}


#ifndef _WIN32
static void intHandler ( int )
{
    if ( vodServer )
    {
        delete vodServer;
    }
    exit ( EXIT_SUCCESS );
}
#endif
