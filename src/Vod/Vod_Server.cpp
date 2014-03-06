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

        std::string tmp;

        uint32_t stream_id;
        std::string stream_name, stream_addr;
        CatalogEntry::Type stream_type;
        CatalogEntry::Protocol stream_protocol;
        uint16_t stream_port;
        uint8_t stream_ips;

        // ID
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 1" << std::endl;
                return false;
            }
            if ( tmp != "ID" )
            {
                std::cerr << "Missing 'ID' at line 1" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'ID:' at line 1" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream ID after 'ID: ' at line 1" << std::endl;
                return false;
            }

            char * endptr;
            unsigned long tmpId = strtoul ( tmp.c_str ( ), &endptr, 10 );

            if ( * endptr != '\0' || tmpId < 1 )
            {
                std::cerr << "Invalid stream ID after 'ID: ' at line 1" << std::endl;
                return false;
            }
            stream_id = ( uint32_t ) tmpId;
        }

        // Name
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 2" << std::endl;
                return false;
            }
            if ( tmp != "Name" )
            {
                std::cerr << "Missing 'Name' at line 2" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'Name:' at line 2" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream Name after 'Name: ' at line 2" << std::endl;
                return false;
            }

            stream_name = tmp;
        }

        // Type
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 3" << std::endl;
                return false;
            }
            if ( tmp != "Type" )
            {
                std::cerr << "Missing 'Type' at line 3" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'Type:' at line 3" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream Type after 'Type: ' at line 3" << std::endl;
                return false;
            }

            if ( ! CatalogEntry::stringToType ( tmp, stream_type ) )
            {
                std::cerr << "Stream Type after 'Type: ' at line 3 is unvalid" << std::endl;
                return false;
            }
        }

        // Address
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 4" << std::endl;
                return false;
            }
            if ( tmp != "Address" )
            {
                std::cerr << "Missing 'Address' at line 4" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'Address:' at line 4" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream Address after 'Address: ' at line 4" << std::endl;
                return false;
            }
            stream_addr = tmp;
        }

        // Port
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 5" << std::endl;
                return false;
            }
            if ( tmp != "Port" )
            {
                std::cerr << "Missing 'Port' at line 5" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'Port:' at line 5" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream Port after 'Port: ' at line 5" << std::endl;
                return false;
            }

            char * endptr;
            unsigned long tmpPort = strtoul ( tmp.c_str ( ), &endptr, 10 );

            if ( * endptr != '\0' || tmpPort < 1 || tmpPort > 65535 )
            {
                std::cerr << "Invalid port after 'Port: ' at line 5" << std::endl;
                return false;
            }
            stream_port = ( uint16_t ) tmpPort;
        }

        // Protocol
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 6" << std::endl;
                return false;
            }
            if ( tmp != "Protocol" )
            {
                std::cerr << "Missing 'Protocol' at line 6" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'Protocol:' at line 6" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream Protocol after 'Protocol: ' at line 6" << std::endl;
                return false;
            }

            if ( ! CatalogEntry::stringToProtocol ( tmp, stream_protocol ) )
            {
                std::cerr << "Stream Protocol after 'Protocol: ' at line 6 is unvalid" << std::endl;
                return false;
            }
        }

        // IPS
        {
            if ( ! std::getline ( f, tmp, ':' ) )
            {
                std::cerr << "Missing ':' at line 7" << std::endl;
                return false;
            }
            if ( tmp != "IPS" )
            {
                std::cerr << "Missing 'IPS' at line 7" << std::endl;
                return false;
            }
            if ( f.get ( ) != ' ' )
            {
                std::cerr << "Missing space after 'IPS:' at line 7" << std::endl;
                return false;
            }
            if ( ! customGetLine ( f, tmp ) )
            {
                std::cerr << "Unable to read stream IPS after 'IPS: ' at line 7" << std::endl;
                return false;
            }

            char * endptr;
            unsigned long tmpIps = strtoul ( tmp.c_str ( ), &endptr, 10 );

            if ( * endptr != '\0' || tmpIps < 1 )
            {
                std::cerr << "Invalid stream IPS after 'IPS: ' at line 7" << std::endl;
                return false;
            }
            stream_ips = ( uint8_t ) tmpIps;
        }

        mcatalogEntries.push_back ( new CatalogEntry (
                    stream_id,
                    stream_name,
                    stream_type,
                    stream_addr,
                    stream_port,
                    stream_protocol,
                    stream_ips ) );

        return true;
    }

    inline std::istream & Server::customGetLine ( std::istream & is, std::string & str )
    {
        std::getline ( is, str );
        if ( str.size ( ) > 0 && str [ str.size ( ) - 1 ] == '\r' )
        {
            str.erase ( str.size ( ) - 1 );
        }

        return is;
    }
}
