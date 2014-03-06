#ifndef _VOD_CATALOG_ENTRY_HPP_
#define _VOD_CATALOG_ENTRY_HPP_

#include <string>

namespace Vod
{
    class CatalogEntry
    {
    public:
        enum Protocol { TCP_PULL, TCP_PUSH, UDP_PULL, UDP_PUSH };
        enum Type { BMP, JPEG };

        CatalogEntry (
                uint32_t id,
                const std::string & name,
                Type type,
                const std::string & addr,
                uint16_t port,
                Protocol proto,
                uint8_t ips );

        virtual ~CatalogEntry ( );

        void toString ( std::ostream & os ) const;

        static void protocolToString ( Protocol proto, std::string & str );
        static bool stringToProtocol ( const std::string & str, Protocol & proto );
        static void typeToString ( Type type, std::string & str );
        static bool stringToType ( const std::string & str, Type & type );


    protected:
        uint32_t mid;
        std::string mname;
        Type mtype;
        std::string maddr;
        uint16_t mport;
        Protocol mproto;
        uint8_t mips;

        std::string mstr;

    private:
        void generateString ( );

        CatalogEntry ( ) = delete;
    };
}

std::ostream & operator<< ( std::ostream & os, const Vod::CatalogEntry & catalogEntry );

#endif
