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
            uint8_t ips ) :
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

    void CatalogEntry::protocolToString ( std::string & str ) const
    {
        switch ( mproto )
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

    void CatalogEntry::typeToString ( std::string & str ) const
    {
        switch ( mtype )
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
        protocolToString ( proto );
        typeToString ( type );

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
