#include "Vod_Request.hpp"
#include <iostream>


namespace Vod
{
    Request::Request ( int sock, const NetFlux::Net::InetAddress & address ) :
        NetFlux::Tcp::ServerStream ( sock, address ),
        reading ( true ),
        endRequest ( 0 ) { }

    Request::~Request ( ) { }

    bool Request::readRequest ( std::string & request )
    {
        if ( endRequest != 4 )
        {
            return false;
        }

        for ( ; ; )
        {
            unsigned int dataSize = mreadBuffer.pageSize ( );

            if ( dataSize == 0 )
            {
                break;
            }

            const char * data = ( const char * ) mreadBuffer.pageContent ( );
            uint8_t end = 0;

            unsigned int i = 0;
            for ( const char * c = data ; i < dataSize ; ++i, ++c )
            {
                request += * c;

                if ( end % 2 == 0 && * c == '\r' )
                {
                    ++end;
                }
                else if ( end % 2 == 1 && * c == '\n' )
                {
                    ++end;
                    if ( end == 4 )
                    {
                        mreadBuffer.acquit ( i + 1 );
                        return true;
                    }
                }
                else
                {
                    end = 0;
                }
            }
            mreadBuffer.acquit ( dataSize );
        }

        return false;
    }

    bool Request::readEventAction ( )
    {
        if ( ! reading )
        {
            std::cout << * this << " : talked during response -> killed" << std::endl;
            delete this;
            return false;
        }

        char buffer [ PAGESIZE ];
        ssize_t ret = recv ( buffer, PAGESIZE );

        if ( ret == -1 )
        {
            std::cout << * this << " : receive error -> killed" << std::endl;
            delete this;
            return false;
        }

        if ( ret == 0 )
        {
            std::cout << * this << " : foreign host closed connection" << std::endl;
            delete this;
            return false;
        }

        std::cout << * this << " : got " << ret << " bytes from foreign host" << std::endl;

        mreadBuffer.append ( buffer, ( unsigned int ) ret );

        ssize_t pos = 0;
        for ( char * cursor = buffer ; pos < ret ; ++cursor, ++pos )
        {
            if ( endRequest % 2 == 0 && * cursor == '\r' )
            {
                endRequest++;
            }
            else if ( endRequest % 2 == 1 && * cursor == '\n' )
            {
                endRequest++;
                if ( endRequest == 4 )
                {
                    requestEventAction ( );
                }
            }
            else
            {
                endRequest = 0;
            }
        }

        return true;
    }

    void Request::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        if ( reading )
        {
            event.setRead ( );
            return;
        }

        event.setWrite ( );
    }

    void Request::toString ( std::ostream & os ) const
    {
        os << "Request socket ";
        NetFlux::Tcp::ServerStream::toString ( os );
    }
}
