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
        // ServerAddress
        {
            Vod_parse ( 1, "ServerAddress", false );
            mpHttpServer -> maddress = tmp;
        }

        // ServerPort
        {
            Vod_parseNumberMinMax ( 2, "ServerPort", false, VOD_MIN_PORT, VOD_MAX_PORT );
            mpHttpServer -> mport = ( uint16_t ) tmpNumber;
        }

        // Catalog Entries
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
}
