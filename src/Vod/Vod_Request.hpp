#ifndef _VOD_REQUEST_HPP_
#define _VOD_REQUEST_HPP_

#include <Tcp/Tcp_ServerStream.hpp>

namespace Vod
{
    class Request : public NetFlux::Tcp::ServerStream
    {
    public:
        Request ( int sock, const NetFlux::Net::InetAddress & address );
        virtual ~Request ( );

        virtual void toString ( std::ostream & os ) const;


    protected:
        virtual bool readEventAction ( );
        virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

        virtual bool requestEventAction ( ) = 0;

        bool reading;


    private:
        Request ( ) = delete;

        uint8_t endRequest;



    };
}

#endif
