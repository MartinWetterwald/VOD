#ifndef _VOD_TCP_CLIENT_HPP_
#define _VOD_TCP_CLIENT_HPP_

#include <Tcp/Tcp_Client.hpp>

namespace Vod
{
    namespace TcpPull
    {
        class ControlConnection;
    }

    class TcpClient : public NetFlux::Tcp::Client
    {
    public:
        TcpClient ( );
        virtual ~TcpClient ( );

    protected:
        virtual bool readEventAction ( );
        virtual bool writeEventAction ( );
        virtual bool exceptEventAction ( );
        virtual bool timeoutEventAction ( );
        virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );

        bool writing;

    private:
        TcpClient ( const TcpClient & ) = delete;
        TcpClient & operator= ( const TcpClient & ) = delete;


    friend class TcpPull::ControlConnection;

    };
}

#endif
