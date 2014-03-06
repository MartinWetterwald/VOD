#include "Vod_CatalogEntry.hpp"
#include <sstream>

namespace Vod
{
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
}

std::ostream & operator<< ( std::ostream & os, const Vod::CatalogEntry & catalogEntry )
{
    catalogEntry.toString ( os );
    return os;
}
