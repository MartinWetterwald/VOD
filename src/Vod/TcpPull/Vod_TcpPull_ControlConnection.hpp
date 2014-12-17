#ifndef _VOD_TCPPULL_STREAM_HPP_
#define _VOD_TCPPULL_STREAM_HPP_

#include "../Vod_Request.hpp"
#include "../Vod_TcpClient.hpp"

namespace Vod
{
    namespace TcpPull
    {
        class Server;

        class ControlConnection : public Request
        {
        public:
            ControlConnection ( int sock, const NetFlux::Net::InetAddress & address );
            ~ControlConnection ( );

        protected:
            virtual bool requestEventAction ( );
            virtual bool writeEventAction ( );
            virtual bool exceptEventAction ( );

            virtual bool timeoutEventAction ( );
            virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

            void toString ( std::ostream & os ) const;

            Server * mpserver;

            bool connected;
            TcpClient streamSocket;
            bool streamSocketWriting;

            bool streamSocketReadEventAction ( TcpClient * socket );
            bool streamSocketWriteEventAction ( TcpClient * socket );
            bool streamSocketExceptEventAction ( TcpClient * socket );
            bool streamSocketTimeoutEventAction ( TcpClient * socket );
            void streamSocketChooseSubscription ( TcpClient * socket, NetFlux::SocketIOEvent::Event & event);

        private:
            ControlConnection ( ) = delete;
            ControlConnection ( const ControlConnection & ) = delete;
            ControlConnection & operator= ( const ControlConnection & ) = delete;


            friend class Server;
        };
    }
}

#endif


