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

    void Buffer::allocate ( unsigned int size, char * data )
    {
        if ( mpdata )
        {
            deallocate ( );
        }

        mpdata = new char [ size ];
        memcpy ( mpdata, data, size );
        msize = size;
    }

    void Buffer::deallocate ( )
    {
        delete [ ] mpdata;
        msize = 0;
        mcursor = 0;
    }

    bool Buffer::acquit ( unsigned int size )
    {
        if ( mcursor + size > msize )
        {
            return false;
        }

        mcursor += size;

        if ( mcursor == size )
        {
            deallocate ( );
        }

        return true;
    }

    char * Buffer::leftDataPointer ( unsigned int & sizeLeft )
    {
        if ( ! mpdata )
        {
            return nullptr;
        }

        sizeLeft = msize - mcursor;

        return mpdata + mcursor;
    }
}
