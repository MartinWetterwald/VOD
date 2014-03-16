#ifndef _VOD_CATALOG_ENTRY_HPP_
#define _VOD_CATALOG_ENTRY_HPP_

#include <string>
#include <map>

namespace Vod
{
    class CatalogEntry
    {
    public:
        enum Protocol { TCP_PULL, TCP_PUSH, UDP_PULL, UDP_PUSH };
        enum Type { BMP, JPEG };

        static CatalogEntry * factory ( const std::string & path );
        virtual ~CatalogEntry ( );

        void toString ( std::ostream & os ) const;

        static void protocolToString ( Protocol proto, std::string & str );
        static bool stringToProtocol ( const std::string & str, Protocol & proto );
        static void typeToString ( Type type, std::string & str );
        static bool stringToType ( const std::string & str, Type & type );


    protected:
        typedef std::map <uint32_t, std::string> CatalogImages;
        CatalogEntry (
                uint32_t id,
                const std::string & name,
                Type type,
                const std::string & addr,
                uint16_t port,
                Protocol proto,
                uint16_t ips,
                CatalogImages * pimages );

        uint32_t mid;
        std::string mname;
        Type mtype;
        std::string maddr;
        uint16_t mport;
        Protocol mproto;
        uint16_t mips;

        CatalogImages * mpimages;

        std::string mstr;

    private:
        void generateString ( );
        static bool parseImage ( std::string & path );

        CatalogEntry ( ) = delete;
        CatalogEntry ( const CatalogEntry & ) = delete;
        CatalogEntry & operator= ( const CatalogEntry & ) = delete;


        friend class Server;
    };
}

std::ostream & operator<< ( std::ostream & os, const Vod::CatalogEntry & catalogEntry );

#endif
