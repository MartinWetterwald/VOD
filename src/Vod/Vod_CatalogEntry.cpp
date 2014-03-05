#include "Vod_CatalogEntry.hpp"

namespace Vod
{
    CatalogEntry::CatalogEntry (
            uint32_t id,
            const std::string & name,
            Type type,
            const std::string & addr,
            uint16_t port,
            Protocol proto ) :
        mid ( id ),
        mname ( name ),
        mtype ( type ),
        maddr ( addr ),
        mport ( port ),
        mproto ( proto ) { }

    CatalogEntry::~CatalogEntry ( )
    {
    }
}
