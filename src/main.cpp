#include <SocketIOEvent/Notifier.hpp>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <csignal>
#include <cstring>
#include <cstdlib>

#include "VODServer.hpp"

#ifndef _WIN32
static void __attribute__ ( ( noreturn ) ) intHandler ( int sig );
#endif

static NetFlux::SocketIOEvent::Notifier * notifier = 0;
static VODServer * vodServer = 0;

int main ( )
{
#ifndef _WIN32
    struct sigaction act;
    memset ( &act, 0, sizeof ( act ) );
    act.sa_handler = intHandler;
    sigaction ( SIGINT, &act, 0 );
#endif

    notifier = new NetFlux::SocketIOEvent::Notifier;

    vodServer = new VODServer;
    if ( ! vodServer -> start ( 8080, notifier ) )
    {
        std::cout << "An error occured while starting the VOD Server."  << std::endl;
        return EXIT_FAILURE;

    }
    notifier -> startNotify ( );

    return EXIT_SUCCESS;
}


#ifndef _WIN32
static void intHandler ( int )
{
    if ( vodServer )
    {
        delete vodServer;
    }

    if ( notifier )
    {
        notifier -> stopNotify ( );
        delete notifier;
    }
    exit ( EXIT_SUCCESS );
}
#endif
