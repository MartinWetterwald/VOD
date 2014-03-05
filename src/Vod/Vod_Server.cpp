#include "Vod_Server.hpp"

#include <SocketIOEvent/SocketIOEvent_Notifier.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Vod
{
    Server::Server ( ) :
        mpNotifier ( new NetFlux::SocketIOEvent::Notifier ),
        mpHttpServer ( new Http::Server ) { }

    Server::~Server ( )
    {
        delete mpHttpServer;
        delete mpNotifier;
    }

    bool Server::start ( )
    {
        if ( ! parseStartUpFile ( ) )
        {
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

    bool Server::parseStartUpFile ( )
    {
        std::ifstream f ( "data/startup.txt", std::ios::binary );

        if ( ! f.is_open ( ) )
        {
            std::cerr << "Unable to open the startup file" << std::endl;
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
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read address after 'ServerAddress: ' at line 1" << std::endl;
                return false;
            }
            if ( tmp.length ( ) == 0 )
            {
                std::cerr << "The address cannot be empty after 'ServerAddress: ' at line 1" << std::endl;
                return false;
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
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read port number after 'ServerPort: ' at line 2" << std::endl;
                return false;
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

        {
            while ( customGetLine ( f, tmp ) )
            {
                if ( ! parseStream ( tmp ) )
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool Server::parseStream ( const std::string & path )
    {
        std::ifstream f ( "data/" + path );

        if ( ! f.is_open ( ) )
        {
            std::cerr << "Unable to open stream file '" << path << "'" << std::endl;
            return false;
        }
        return true;
    }

    inline std::istream & Server::customGetLine ( std::istream & is, std::string & str )
    {
        std::getline ( is, str );
        if ( str [ str.size ( ) - 1 ] == '\r' )
        {
            str.erase ( str.size ( ) - 1 );
        }

        return is;
    }
}
