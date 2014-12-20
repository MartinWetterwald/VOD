#ifndef _VOD_BUFFER_HPP_
#define _VOD_BUFFER_HPP_

namespace Vod
{
    class Buffer
    {
    public:
        Buffer ( );
        virtual ~Buffer ( );

        void allocate ( unsigned int size, char * data );
        void deallocate ( );
        bool acquit ( unsigned int size );

        char * leftDataPointer ( unsigned int & sizeLeft );


    protected:
        char * mpdata;
        unsigned int msize;
        unsigned int mcursor;
    };
}

#endif
