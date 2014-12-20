#ifndef _VOD_BUFFER_HPP_
#define _VOD_BUFFER_HPP_

#include <cstdint>

namespace Vod
{
    class Buffer
    {
    public:
        Buffer ( );
        virtual ~Buffer ( );

        void allocate ( uint64_t size );
        void deallocate ( );

        bool acquit ( uint64_t size )
        {
            if ( mcursor + size > msize )
            {
                return false;
            }

            mcursor += size;

            return true;
        }

        inline bool end ( )
        {
            return mcursor == msize;
        }

        bool memcpy ( uint64_t pos, char * src, uint64_t size );

        char * leftDataPointer ( uint64_t & sizeLeft );

        inline char * data ( )
        {
            return mpdata;
        }


    protected:
        char * mpdata;
        uint64_t msize;
        uint64_t mcursor;
    };
}

#endif
