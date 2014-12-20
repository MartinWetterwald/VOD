#ifndef _VOD_CATALOG_ENTRY_HPP_
#define _VOD_CATALOG_ENTRY_HPP_

#include <string>
#include <map>

namespace Vod
{
    class Server;
    class Buffer;

    namespace TcpPull
    {
        class ControlConnection;
    }

    class CatalogEntry
    {
    public:
        enum Protocol { TCP_PULL, TCP_PUSH, UDP_PULL, UDP_PUSH };
        enum Type { BMP, JPEG };

        CatalogEntry ( ) = default;
        static CatalogEntry * factory ( const std::string & path, Server * server );
        virtual ~CatalogEntry ( );

        virtual bool start ( ) = 0;

        virtual void toTitleString ( std::ostream & os ) const;
        void toCatalogString ( std::ostream & os ) const;

        static void protocolToString ( Protocol proto, std::string & str );
        static bool stringToProtocol ( const std::string & str, Protocol & proto );
        static void typeToString ( Type type, std::string & str );
        static bool stringToType ( const std::string & str, Type & type );

        bool loadFrame ( uint32_t frameId, Buffer & destBuf ) const;

        inline uint64_t frameTotal ( ) const
        {
            if ( ! mpimages )
            {
                return 0;
            }

            return mpimages -> size ( );
        }


    protected:
        typedef std::map <uint32_t, std::string> CatalogImages;

        uint32_t mid;
        std::string mname;
        Type mtype;
        std::string maddr;
        uint16_t mport;
        Protocol mproto;
        uint16_t mips;

        Server * mpvodServer;
        CatalogImages * mpimages;

        std::string mstr;

    private:
        inline void fill (
            uint32_t id,
            const std::string & name,
            Type type,
            const std::string & addr,
            uint16_t port,
            Protocol proto,
            uint16_t ips,
            CatalogImages * pimages,
            Server * pserver )
        {
            mid = id;
            mname = name;
            mtype = type;
            maddr = addr;
            mport = port;
            mproto = proto;
            mips = ips;
            mpimages = pimages;
            mpvodServer = pserver;
            generateString ( );
        }

        void generateString ( );
        static bool parseImage ( std::string & path );

        CatalogEntry ( const CatalogEntry & ) = delete;
        CatalogEntry & operator= ( const CatalogEntry & ) = delete;


        friend class Server;
    };
}

std::ostream & operator<< ( std::ostream & os, const Vod::CatalogEntry & catalogEntry );

#endif
