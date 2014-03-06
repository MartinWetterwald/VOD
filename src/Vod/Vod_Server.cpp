#include "Vod_Server.hpp"
#include "Vod_parsingUtils.hpp"

#include <SocketIOEvent/SocketIOEvent_Notifier.hpp>
#include <iostream>

namespace Vod
{
    Server::Server ( ) :
        mpNotifier ( new NetFlux::SocketIOEvent::Notifier ),
        mpHttpServer ( new Http::Server ( this ) ) { }

    Server::~Server ( )
    {
        delete mpHttpServer;
        delete mpNotifier;

        for ( const auto & pcatalogEntry : mcatalogEntries )
        {
            delete pcatalogEntry;
        }
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
        std::cout << "Parsing startup file..." << std::endl;

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

            if ( * endptr != '\0' || tmpPort < 1 || tmpPort > 65535 )
            {
                std::cerr << "Invalid port after 'ServerPort: ' at line 2" << std::endl;
                return false;
            }
            mpHttpServer -> mport = ( uint16_t ) tmpPort;
        }

        {
            while ( customGetLine ( f, tmp ) )
            {
                std::cout << "Parsing stream file " << tmp << "..." << std::endl;
                CatalogEntry * pcatalogEntry = CatalogEntry::factory ( tmp );
                if ( pcatalogEntry == nullptr )
                {
                    return false;
                }

                mcatalogEntries.push_back ( pcatalogEntry );
            }
        }

        return true;
    }

    bool Server::parseImage ( std::string & path )
    {
#ifndef _WIN32
        size_t pos = 0;
        const std::string search ( "\\" );
        const std::string replace ( "/" );
        while ( ( pos = path.find ( search, pos ) ) != std::string::npos )
        {
            path.replace ( pos, search.length ( ), replace );
            pos += replace.length ( );
        }
#endif
        std::ifstream f ( "data/" + path );

        if ( ! f.is_open ( ) )
        {
            std::cerr << "Unable to open image file '" << path << "'" << std::endl;
            return false;
        }

        return true;
    }

}
