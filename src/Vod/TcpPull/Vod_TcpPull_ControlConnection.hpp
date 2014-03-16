#ifndef _VOD_TCPPULL_STREAM_HPP_
#define _VOD_TCPPULL_STREAM_HPP_

#include <Tcp/Tcp_ServerStream.hpp>

namespace Vod
{
    namespace TcpPull
    {
        class Server;

        class ControlConnection : public NetFlux::Tcp::ServerStream
        {
        public:
            ControlConnection ( int sock, const NetFlux::Net::InetAddress & address );
            ~ControlConnection ( );

        protected:
            virtual bool readEventAction ( );
            virtual bool writeEventAction ( );
            virtual bool exceptEventAction ( );

            virtual bool timeoutEventAction ( );
            virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

            void toString ( std::ostream & os ) const;

            Server * mpserver;
            uint32_t mcursor;

        private:
            ControlConnection ( ) = delete;
            ControlConnection ( const ControlConnection & ) = delete;
            ControlConnection & operator= ( const ControlConnection & ) = delete;


            friend class Server;
        };
    }
}

#endif


