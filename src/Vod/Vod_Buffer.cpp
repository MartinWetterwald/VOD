#include "Vod_Buffer.hpp"
#include <cstring>

namespace Vod
{
    Buffer::Buffer ( ) : mpdata ( nullptr ), msize ( 0 ), mcursor ( 0 )
    {

    }

    Buffer::~Buffer ( )
    {
        delete [ ] mpdata;
    }

    void Buffer::allocate ( uint64_t size )
    {
        if ( mpdata )
        {
            deallocate ( );
        }

        mpdata = new char [ size ];
        msize = size;
    }

    void Buffer::deallocate ( )
    {
        if ( ! mpdata )
        {
            return;
        }

        delete [ ] mpdata;

        mpdata = nullptr;
        msize = 0;
        mcursor = 0;
    }

    bool Buffer::memcpy ( uint64_t pos, char * src, uint64_t size )
    {
        if ( ! mpdata || pos + size > msize )
        {
            return false;
        }

        ::memcpy ( mpdata + pos, src, size );

        return true;
    }

    char * Buffer::leftDataPointer ( uint64_t & sizeLeft )
    {
        if ( ! mpdata )
        {
            return nullptr;
        }

        sizeLeft = msize - mcursor;

        return mpdata + mcursor;
    }
}
