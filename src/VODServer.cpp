#include "VODServer.hpp"

#include <SocketIOEvent/Notifier.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

VODServer::VODServer ( )
{
    mpNotifier = new NetFlux::SocketIOEvent::Notifier;
    mpHttpServer = new HttpServer;
}

VODServer::~VODServer ( )
{
    delete mpHttpServer;
    delete mpNotifier;
}

bool VODServer::start ( )
{
    if ( ! parseStartUpFile ( ) )
    {
        std::cerr << "An error occured while trying to parse the startup file." << std::endl;
        return false;
    }

    mpHttpServer -> generateCatalog ( );

    if ( ! mpHttpServer -> listen ( ) )
    {
        return false;
    }
    std::cout << "HTTP Server listening on " << * mpHttpServer << std::endl;

    mpNotifier -> subscribe ( mpHttpServer );

    mpNotifier -> startNotify ( );
    return true;
}

bool VODServer::parseStartUpFile ( )
{
    std::ifstream f ( "data/startup.txt", std::ios::binary );

    if ( ! f.is_open ( ) )
    {
        std::cerr << "Unable to open the file" << std::endl;
        return false;
    }

    std::string tmp;
    {
        if ( ! std::getline ( f, tmp, ':' ) )
        {
            std::cerr << "Missing ':' at line 1" << std::endl;
            return false;
        }
        if ( tmp != "ServerAddress" )
        {
            std::cerr << "Missing 'ServerAddress' at line 1" << std::endl;
            return false;
        }
        if ( f.get ( ) != ' ' )
        {
            std::cerr << "Missing space after 'ServerAddress:' at line 1" << std::endl;
            return false;
        }
        if ( ! std::getline ( f, tmp ) )
        {
            return false;
        }
        if ( tmp [ tmp.size ( ) - 1 ] == '\r' )
        {
            tmp.erase ( tmp.size ( ) - 1 );
        }
        mpHttpServer -> maddress = tmp;
    }

    {
        const uint16_t MINPORT = 1;
        const uint16_t MAXPORT = 65535;
        if ( ! std::getline ( f, tmp, ':' ) )
        {
            std::cerr << "Missing ':' at line 2" << std::endl;
            return false;
        }
        if ( tmp != "ServerPort" )
        {
            std::cerr << "Missing 'ServerPort' at line 2" << std::endl;
            return false;
        }
        if ( f.get ( ) != ' ' )
        {
            std::cerr << "Missing space after 'ServerPort:' at line 2" << std::endl;
            return false;
        }
        if ( ! std::getline ( f, tmp ) )
        {
            return false;
        }
        if ( tmp [ tmp.size ( ) - 1 ] == '\r' )
        {
            tmp.erase ( tmp.size ( ) - 1 );
        }

        char * endptr;
        unsigned long tmpPort = strtoul ( tmp.c_str ( ), &endptr, 10 );

        if ( * endptr != '\0' || tmpPort < MINPORT || tmpPort > MAXPORT )
        {
            std::cerr << "Invalid port after 'ServerPort: ' at line 2" << std::endl;
            return false;
        }
        mpHttpServer -> mport = ( uint16_t ) tmpPort;
    }

    return true;
}
