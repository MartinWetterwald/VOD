#include "Vod_CatalogEntry.hpp"
#include "Vod_parsingUtils.hpp"
#include <iostream>

namespace Vod
{
    CatalogEntry * CatalogEntry::factory ( const std::string & path )
    {
        std::ifstream f ( "data/" + path );

        if ( ! f.is_open ( ) )
        {
            std::cerr << "Unable to open stream file '" << path << "'" << std::endl;
            return nullptr;
        }

        std::string tmp;

        uint32_t stream_id;
        std::string stream_name, stream_addr;
        CatalogEntry::Type stream_type;
        CatalogEntry::Protocol stream_protocol;
        uint16_t stream_port;
        uint16_t stream_ips;

        // ID
        {
            Vod_parseNumber ( 1, "ID", nullptr, 1, 5 );
            stream_id = ( uint32_t ) tmpNumber;
        }

        // Name
        {
            Vod_parse ( 2, "Name", nullptr );
            stream_name = tmp;
        }

        // Type
        {
            Vod_parse ( 3, "Type", nullptr );
            if ( ! CatalogEntry::stringToType ( tmp, stream_type ) )
            {
                std::cerr << "Stream Type after 'Type: ' at line 3 is unvalid" << std::endl;
                return nullptr;
            }
        }

        // Address
        {
            Vod_parse ( 4, "Address", nullptr );
            stream_addr = tmp;
        }

        // Port
        {
            Vod_parseNumber ( 5, "Port", nullptr, VOD_MIN_PORT, VOD_MAX_PORT );
            stream_port = ( uint16_t ) tmpNumber;
        }

        // Protocol
        {
            Vod_parse ( 6, "Protocol", nullptr );
            if ( ! CatalogEntry::stringToProtocol ( tmp, stream_protocol ) )
            {
                std::cerr << "Stream Protocol after 'Protocol: ' at line 6 is unvalid" << std::endl;
                return nullptr;
            }
        }

        // IPS
        {
            Vod_parseNumber ( 7, "IPS", nullptr, 1, 50 );
            stream_ips = ( uint16_t ) tmpNumber;
        }

        // Image files
        {
            while ( customGetLine ( f, tmp ) )
            {
                if ( ! parseImage ( tmp ) )
                {
                    return nullptr;
                }
            }
        }

        return new CatalogEntry (
            stream_id,
            stream_name,
            stream_type,
            stream_addr,
            stream_port,
            stream_protocol,
            stream_ips );
    }

    CatalogEntry::~CatalogEntry ( ) { }

    void CatalogEntry::toString ( std::ostream & os ) const
    {
        os << mstr;
    }

    void CatalogEntry::protocolToString ( Protocol proto, std::string & str )
    {
        switch ( proto )
        {
            case TCP_PULL:
                str = "TCP_PULL";
                return;

            case TCP_PUSH:
                str = "TCP_PUSH";
                return;

            case UDP_PULL:
                str = "UDP_PULL";
                return;

            case UDP_PUSH:
                str = "UDP_PUSH";
                return;
        }
    }

    bool CatalogEntry::stringToProtocol ( const std::string & str, Protocol & proto )
    {
        if ( str == "TCP_PULL" )
        {
            proto = TCP_PULL;
        }
        else if ( str == "TCP_PUSH" )
        {
            proto = TCP_PUSH;
        }
        else if ( str == "UDP_PULL" )
        {
            proto = UDP_PULL;
        }
        else if ( str == "UDP_PUSH" )
        {
            proto = UDP_PUSH;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool CatalogEntry::stringToType ( const std::string & str, Type & type )
    {
        if ( str == "BMP" )
        {
            type = BMP;
        }
        else if ( str == "JPEG" )
        {
            type = JPEG;
        }
        else
        {
            return false;
        }

        return true;
    }

    void CatalogEntry::typeToString ( Type type, std::string & str )
    {
        switch ( type )
        {
            case BMP:
                str = "BMP";
                return;

            case JPEG:
                str = "JPEG";
                return;
        }
    }

    CatalogEntry::CatalogEntry (
            uint32_t id,
            const std::string & name,
            Type type,
            const std::string & addr,
            uint16_t port,
            Protocol proto,
            uint16_t ips ) :
        mid ( id ),
        mname ( name ),
        mtype ( type ),
        maddr ( addr ),
        mport ( port ),
        mproto ( proto ),
        mips ( ips )
    {
        generateString ( );
    }

    void CatalogEntry::generateString ( )
    {
        std::string proto, type;
        protocolToString ( mproto, proto );
        typeToString ( mtype, type );

        std::ostringstream ss;
        ss
            << "Object "
            << "ID=" << mid << " "
            << "name=" << mname << " "
            << "type=" << type << " "
            << "address=" << maddr << " "
            << "port=" << mport << " "
            << "protocol=" << proto << " "
            << "ips=" << mips << "\r\n";

        mstr = ss.str ( );
    }

    bool CatalogEntry::parseImage ( std::string & path )
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

std::ostream & operator<< ( std::ostream & os, const Vod::CatalogEntry & catalogEntry )
{
    catalogEntry.toString ( os );
    return os;
}
